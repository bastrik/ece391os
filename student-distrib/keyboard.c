/*
 * keyboard.c
 * This is a keyboard driver for SamplersOS
 */

#include "types.h"
#include "i8259.h"
#include "lib.h"
#include "keyboard.h"

/* the number of characters on a keyboard */
#define KEY_CHAR_NUM 64

/* keyboard characters at normal */
static uint8_t keyboard_char_norm[KEY_CHAR_NUM] =
  {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
  '\0', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
  '\0', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l' , ';', '\'', '`',
  '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',',', '.', '/', '\0', '*',
  '\0', ' ', '\0'};

/* keyboard characters when Shift key pressed */
static uint8_t keyboard_char_shift[KEY_CHAR_NUM] =
  {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
'\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
'\0', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L' , ':', '"', '~',
'\0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '\0', '*',
'\0', ' ', '\0'};

/* keyboard characters when CapsLock key pressed */
static uint8_t keyboard_char_caps[KEY_CHAR_NUM] =
  {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
'\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
'\0', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L' , ';', '\'', '`',
'\0', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', '\0', '*',
'\0', ' ', '\0'};

/* keyboard characters when both CapsLock and Shift keys pressed */
static uint8_t keyboard_char_both[KEY_CHAR_NUM] =
  {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
'\0', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}',
'\0', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l' , ':', '"', '~',
'\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', '\0', '*',
'\0', ' ', '\0'};

/* flags for shift, caps, ctrl, alt */


/*
 * keyboard_init
 *   DESCRIPTION: Initializes the keyboard
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Enables IRQ on PIC
 *                  and initializes variables
 */
void keyboard_init(void) {
  enable_irq(KEYBOARD_IRQ_NUM);
  return;
}

/*
 * keyboard_handler
 *   DESCRIPTION: Obatins a key information from the keyboard_handler
 *                  then prints it on the screen
 *   INPUTS: none
 *   OUTPUTS: Appropriate character gets printed on the screen
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Disables interrupts during its tasks
 *                 Make PIC to send EOI signal
 */
void keyboard_handler() {
  /* Beginning of a critical section  */
  cli();
  uint8_t scancode = 0;

  /* make sure the scancode is recieved */
  do {
    scancode = inb(KEYBOARD_PORT_DATA);
    if (scancode > 0){
      break;
    }
  }while(1);

  putc(keyboard_char_norm[scancode]);

  send_eoi(KEYBOARD_IRQ_NUM);
  sti();
  /* End of the critical section */
  return;
}
