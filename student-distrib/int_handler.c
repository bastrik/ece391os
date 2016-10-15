/*
 * 	int_handler.c
 * 
 *  contains all the interrupt handler function (will be called from idt.c)
 *
 */

/* Using this trick to define similar functions with dynamic function names  	*														*
 * See the link below for more detail											*
 * http://stackoverflow.com/questions/7561828/variable-name-from-macro-argument	*/

#define GENERATE_HANDLER(func, log) \
void func(){ 						\
 	printf("%s\n", #log);			\
 	while(1);						\
}

/* Interrupt 0: Divide Zero Exception */
GENERATE_HANDLER(de_handler, "Exception: Divide Zero Error");
/* Interrupt 1: Debug Exception */
GENERATE_HANDLER(db_handler, "Exception: Debug");
/* Interrupt 2: Nonmaskable Interrupt Exception */
GENERATE_HANDLER(nmi_handler, "Exception: Nonmaskable Interrupt");
/* Interrupt 3: Breakpoint Exception */
GENERATE_HANDLER(bp_handler, "Exception: Breakpoint");
/* Interrupt 4: Overflow Exception */
GENERATE_HANDLER(of_handler, "Exception: Overflow");
/* Interrupt 5: Bound range exceeded Exception */
GENERATE_HANDLER(br_handler, "Exception: Bound range exceeded");
/* Interrupt 6: Invalid Opcode Exception */
GENERATE_HANDLER(ud_handler, "Exception: Invalid Opcode");
/* Interrupt 7: Device Not Available Exception */
GENERATE_HANDLER(nm_handler, "Exception: Device Not Available");
/* Interrupt 8: Double Fault Exception */
GENERATE_HANDLER(df_handler, "Exception: Double Fault");
/* Interrupt 9: Coprocessor Segment Overrun Exception */
GENERATE_HANDLER(cs_handler, "Exception: Coprocessor Segment Overrun");
/* Interrupt 10: Invalid TSS Exception */
GENERATE_HANDLER(ts_handler, "Exception: Invalid TSS Exception");
/* Interrupt 11: Segment Not Present */
GENERATE_HANDLER(np_handler, "Exception: Segment Not Present");
/* Interrupt 12: Stack Fault Exception */
GENERATE_HANDLER(ss_handler, "Exception: Stack Fault");
/* Interrupt 13: General Protection Exception */
GENERATE_HANDLER(gp_handler, "Exception: General Protection");
/* Interrupt 14: Page-Fault Exception */
GENERATE_HANDLER(pf_handler, "Exception: Page-Fault");
/* Interrupt 16: x87 FPU Floating-Point Error Exception */
GENERATE_HANDLER(mf_handler, "Exception: x87 FPU Floating-Point Error");
/* Interrupt 17: Alignment Check Exception */
GENERATE_HANDLER(ac_handler, "Exception: Alignment Check Error");
/* Interrupt 18: Machine-Check Exception */
GENERATE_HANDLER(mc_handler, "Exception: Machine-Check Error");
/* Interrupt 19: SIMD Floating-Point Exception Exception */
GENERATE_HANDLER(xf_handler, "Exception: SIMD Floating-Point Exception Error");


/* 
 *	invalid_interrupt()
 *  DESCRIPTION: the default interrupt handler, called from undefined entries of the IDT
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: none
 *  SIDE EFFECTS: none
 */

 void invalid_interrupt(){
 	printf("Error: Interrupt invalid");
 }
