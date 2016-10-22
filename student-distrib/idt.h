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

/* Interrupt 0: Divide Zero Exception */
extern void de_handler();
/* Interrupt 1: Debug Exception */
extern void db_handler();
/* Interrupt 2: Nonmaskable Interrupt Exception */
extern void nmi_handler();
/* Interrupt 3: Breakpoint Exception */
extern void bp_handler();
/* Interrupt 4: Overflow Exception */
extern void of_handler();
/* Interrupt 5: Bound range exceeded Exception */
extern void br_handler();
/* Interrupt 6: Invalid Opcode Exception */
extern void ud_handler();
/* Interrupt 7: Device Not Available Exception */
extern void nm_handler();
/* Interrupt 8: Double Fault Exception */
extern void df_handler();
/* Interrupt 9: Coprocessor Segment Overrun Exception */
extern void cs_handler();
/* Interrupt 10: Invalid TSS Exception */
extern void ts_handler();
/* Interrupt 11: Segment Not Present */
extern void np_handler();
/* Interrupt 12: Stack Fault Exception */
extern void ss_handler();
/* Interrupt 13: General Protection Exception */
extern void gp_handler();
/* Interrupt 14: Page-Fault Exception */
extern void pf_handler();
/* Interrupt 16: x87 FPU Floating-Point Error Exception */
extern void mf_handler();
/* Interrupt 17: Alignment Check Exception */
extern void ac_handler();
/* Interrupt 18: Machine-Check Exception */
extern void mc_handler();
/* Interrupt 19: SIMD Floating-Point Exception Exception */
extern void xf_handler();

#endif /* IDT_H */
