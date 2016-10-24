/*
 * keyboard.c
 * This is a keyboard driver for SamplersOS
 */

#include "keyboard.h"
#include "types.h"
#include "i8259.h"
#include "lib.h"
#include "terminal.h"
//#include "system_calls.h"

/* string buffer to be displayed on the screen */
static uint8_t key_buffer[KEY_BUFF_LEN];
static uint32_t key_buffer_idx;

/* key flags for shift, capslock, ctrl respectively */
static uint8_t key_flag[KEY_FLAG_NUM];
volatile uint8_t enter_flag;

/* the fowlloing key mapping corresponds to the scan code set 1
 * the keys that this driver doesn't support is set as NULL ('\0')
 *
 * following key maps correspond to the scan code set 1*/
static uint8_t keyboard_char_norm[KEY_CHAR_NUM] =
  {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
  '\0', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
  '\0', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l' , ';', '\'', '`',
  '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',',', '.', '/', '\0', '*',
  '\0', ' ', '\0'};

/* keyboard characters when CapsLock key pressed */
static uint8_t keyboard_char_caps[KEY_CHAR_NUM] =
  {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
  '\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
  '\0', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L' , ';', '\'', '`',
  '\0', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', '\0', '*',
  '\0', ' ', '\0'};

/* keyboard characters when Shift key pressed */
static uint8_t keyboard_char_shift[KEY_CHAR_NUM] =
  {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
  '\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
  '\0', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L' , ':', '"', '~',
  '\0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '\0', '*',
  '\0', ' ', '\0'};

/* keyboard characters when both CapsLock and Shift keys pressed */
static uint8_t keyboard_char_both[KEY_CHAR_NUM] =
  {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
  '\0', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}',
  '\0', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l' , ':', '"', '~',
  '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', '\0', '*',
  '\0', ' ', '\0'};



/*
 * keyboard_init()
 *   DESCRIPTION: Initializes the keyboard
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Enables IRQ on PIC
 *                  and initializes variables
 */
void keyboard_init(void) {
  enable_irq(KEYBOARD_IRQ_NUM);
  /* initialize the key flags for shift, capslock, ctrl */
  int i;
  for (i = 0; i < KEY_FLAG_NUM; i++) {
    key_flag[i] = 0;
  }
  /* initialize the key_buffer and the index */
  for (i = 0; i < KEY_BUFF_LEN; i++) {
    key_buffer[i] = '\0';
  }
  key_buffer_idx = 0;
  enter_flag = 0;
}

/*
 * keyboard_handler()
 *   DESCRIPTION: Obatins a scancode from keyboard
 *               then apply the scancode
 *   INPUTS: none
 *   OUTPUTS: Appropriate character gets printed on the screen
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Disables interrupts during its tasks
 *                 Sends EOI signal
 *                 Changes a value in the key_flag array
 *
 */
void keyboard_handler() {
  /* Beginning of a critical section  */
  cli();
  uint8_t scancode = 0;

  /* obtaining scan code from keyboard
   * credit to PS2/2 Keyboard - OSDev Wiki
   * http://wiki.osdev.org/Keyboard
   */
  do {
    if (inb(KEYBOARD_DATA_PORT)!= scancode) {
      scancode = inb(KEYBOARD_DATA_PORT);
      if (scancode < 1) {
        break;
      }
    }
  } while(1);

  /* handles the obtained scan code */
  switch (scancode) {
    case ENTER:
      enter_handler();
      break;
    case CAPSLOCK:
      key_flag[CAPS] = PRESS;
      break;
    case LSHIFT_PRESS:
      key_flag[SHIFT] = PRESS;
      break;
    case LSHIFT_RELEASE:
      key_flag[SHIFT] = RELEASE;
      break;
    case RSHIFT_PRESS:
      key_flag[SHIFT] = PRESS;
      break;
    case RSHIFT_RELEASE:
      key_flag[SHIFT] = RELEASE;
      break;
    case LCTRL_PRESS:
      key_flag[CTRL] = PRESS;
      break;
    case LCTRL_RELEASE:
      key_flag[CTRL] = RELEASE;
      break;
    case BACKSPACE:
      backspace_hander();
      break;
    default:
      key_to_buffer(scancode);
      break;
  }

  /* system call check */
  //if (scancode < 0x80)
  //  putc(keyboard_char_norm[scancode]);
  send_eoi(KEYBOARD_IRQ_NUM);

  /* End of the critical section */
  sti();
}

/*
 * key_to_buffer(uint8_t scancode)
 *   DESCRIPTION: Pushes a charcter related to the recieved scancode
 *             into the key buffer and displays the character on the screen
 *   INPUTS: scancode -- scan code received from keyboard
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Charcter related to the recieved scancode value
 *            is pushed into the key buffer and displayed on the screen
 */
void key_to_buffer(uint8_t scancode) {
  uint8_t key = 0;

  /* checks key map boundary, then selects key map with key flags */
  if (scancode < KEY_CHAR_NUM) {
    if (key_flag[CAPS] == PRESS && key_flag[SHIFT] == RELEASE)
      key = keyboard_char_caps[scancode];
    else if (key_flag[CAPS] == RELEASE && key_flag[SHIFT] == PRESS)
      key = keyboard_char_shift[scancode];
    else if (key_flag[CAPS] == PRESS && key_flag[SHIFT] == PRESS)
      key = keyboard_char_both[scancode];
    else
      key = keyboard_char_norm[scancode];
  }
  else
    return;

  /* key not supported */
  if (key == '\0')
    return;

  /* CTRL+L, clear the screen and put the cursor at the top */
  if (key_flag[CTRL] == PRESS) {
    if (key == 'l' || key == 'L') {
      clear();
      set_cursor(0, 0);
      return;
    }
  }

  /* append to the key buffer and display the key on the screen */
  if (key_buffer_idx < KEY_BUFF_LEN) {
    key_buffer_idx++;
		key_buffer[key_buffer_idx] = key;
    putc(key);
	}

  /* nothing changed */
  return;
}

/*
 * enter_handler()
 *   DESCRIPTION: ENTER key functionality
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Prints the string stored in the key_buffer on the screen
 *                 Moves a curosr one line downward
 *                 Moves the screen vertically if neccessary
 */
void enter_handler() {
  key_buffer[key_buffer_idx++] = '\n';
  set_cursor_enter();
  enter_flag = 1;
  return;
}

/*
 * backspace_hander()
 *   DESCRIPTION: BACKSPACE key functionality
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Erases the displayed character right before the cursor
 *                 Moves a curosr to the left one time
 *                 erases the last element in the key_buffer
 */
void backspace_hander() {
  if (key_buffer_idx != 0) {
		key_buffer_idx--;
		key_buffer[key_buffer_idx] = '\0';
    set_cursor_backspace();
	}
  return;
}

/*
 * int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes)
 *   DESCRIPTION: read system call for terminal driver
 *   INPUTS: fd -- file descriptor
 *           buf -- buffer to fill
 *           nbytes -- the number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: the number of bytes read
 *   SIDE EFFECTS: clears the key_buffer
 */
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes) {
  sti();

  /* wait for the ENTER key to be pressed */
  while(enter_flag == 0)
    enter_flag = 0;

  int i, retval;

  /* fill the buffer */
  for (i = 0; (i < nbytes-1) && (i < KEY_BUFF_LEN); i++) {
		buf[i] = key_buffer[i];
    if (key_buffer[i] == '\0')
      break;
  }

  retval = i;

  /* clear the key_buffer */
  for (i = 0; i < KEY_BUFF_LEN; i++) {
    key_buffer[i] = '\0';
  }

  return retval;
}

/*
 * int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes)
 *   DESCRIPTION: write system call for terminal driver
 *   INPUTS: fd -- file descriptor
 *           buf -- buffer to write with
 *           nbytes -- the number of bytes to display
 *   OUTPUTS: data in the given buf displayed
 *   RETURN VALUE: the number of bytes written
 *   SIDE EFFECTS: none
 */
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes) {
  int retval = printf((int8_t *)buf);
	return retval;
}

/*
 * int32_t terminal_open(const uint8_t* filename)
 *   DESCRIPTION: open system call for terminal driver
 *   INPUTS: filename -- file to open
 *   OUTPUTS: none
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: none
 */
int32_t terminal_open(const uint8_t* filename) {
  return 0;
}
/*
 * int32_t terminal_close(int32_t fd)
 *   DESCRIPTION: close system call for terminal driver
 *   INPUTS: fd -- specified file descriptor
 *   OUTPUTS: none
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: none
 */
int32_t terminal_close(int32_t fd) {
  return 0;
}
