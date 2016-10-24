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

 #define KEYBOARD_DATA_PORT 0x60
 #define KEYBOARD_IRQ_NUM   1
 #define KEYBOARD_IDT_VEC   0x21

 extern void keyboard_init();
 extern void keyboard_handler();

 #endif /* _KEYBOARD_H */
