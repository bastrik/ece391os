#ifndef _IDT_H
#define _IDT_H

#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "keyboard.h"

void init_idt();

extern void division_by_zero();
extern void debugger();
extern void non_maskable_interrupt();
extern void breakpoint();
extern void overflow();
extern void bounds();
extern void invalid_opcode();
extern void coprocessor_not_available();
extern void double_fault();
extern void coprocessor_segment_overrun();
extern void invalid_task_state_segment();
extern void segment_not_present();
extern void stack_fault();
extern void general_protection_fault();
extern void page_fault();
extern void x87_FPU_error();
extern void alignment_check();
extern void machine_check();
extern void SIMD_floating_point_exception();
extern void ignore_int();


#endif /* _IDT_H */

