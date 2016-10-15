/*
 * idt.h 
 * 
 * Functions to handle interrupt descriptor table
 *
 */

#ifndef IDT_H
#define IDT_H

#define SYS_CALL_INT 0x80


/* initialize the content of interrupt descriptor table */
void init_idt();

#endif
