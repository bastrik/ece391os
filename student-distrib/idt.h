/*
 * idt.h 
 * 
 * Functions to handle interrupt descriptor table
 *
 */

#ifndef _IDT_H
#define _IDT_H

#define SYS_CALL_INT 0x80


/* initialize the content of interrupt descriptor table */
extern void init_idt();