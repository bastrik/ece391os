/* asm_link.h - Defines for various x86 descriptors, descriptor tables, 
 * and selectors
 * vim:ts=4 noexpandtab
 */

#ifndef _ASM_LINK_H
#define _ASM_LINK_H

#include "types.h"


/* Call keyboard_handler function declared in asm_link.S */
extern void keyboard_handler_asm();
/* Call rtc_handler function declared in asm_link.S */
extern void rtc_handler_asm();
/* Call systemcall_handler function declared in asm_link.S */
extern void systemcall_handler_asm();

#endif 

