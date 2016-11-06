/*
* keyboard.h
* This is a header file of the
*  keyboard driver for SamplersOS
*/

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#include "lib.h"
#include "i8259.h"
#include "filesys.h"
#include "system_call.h"
#include "rtc.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_IRQ_NUM   1

/* the number of characters on a keyboard */
#define KEY_CHAR_NUM 128
#define KEY_FLAG_NUM 3
#define KEY_BUFF_LEN 128

/* for key flags */
#define CAPS  0x00
#define SHIFT 0x01
#define CTRL  0x02
#define PRESS   0x01
#define RELEASE 0x00

/* keys with functionality */
#define ENTER       0x1C
#define CAPSLOCK    0x3A
#define LSHIFT_PRESS    0x2A
#define LSHIFT_RELEASE  0xAA
#define RSHIFT_PRESS    0x36
#define RSHIFT_RELEASE  0xB6
#define LCTRL_PRESS     0x1D
#define LCTRL_RELEASE   0x9D
#define BACKSPACE       0x0E

/* Initializes the keyboard */
extern void keyboard_init();
void key_to_buffer(uint8_t scancode);
/* Handler for keyboard interrupt */
extern void keyboard_handler();
/* Handler for ENTER key */
extern void enter_handler();
/* Handler for BACKSPACE key */
void backspace_hander();

/* SYSTEM CALL functions for terminal */
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t terminal_open(const uint8_t* filename);
int32_t terminal_close(int32_t fd);

// TEST FUNCTION
void keyboard_test();

#endif /* _KEYBOARD_H */
