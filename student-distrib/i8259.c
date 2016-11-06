/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts
 * are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7 */
uint8_t slave_mask; /* IRQs 8-15 */

#define MASK_ALL 0xFF
#define PORT_NUM 8
#define MASTER_DATA_PORT   0x21
#define SLAVE_DATA_PORT  0xA1

/* Initialize the 8259 PIC */

 /* 
 * i8259_inti
 * DESCRIPTION: send control words to sepcific port
 	and then maske all the interrupts;
 * 
 * 
 * 
 * 
 */
void
i8259_init(void)
{
	outb(ICW1,MASTER_8259_PORT);                  // send control words
	outb(ICW1,SLAVE_8259_PORT);

	outb(ICW2_MASTER, MASTER_DATA_PORT);
	outb(ICW2_SLAVE, SLAVE_DATA_PORT);

	outb(ICW3_MASTER,MASTER_DATA_PORT);
	outb(ICW3_SLAVE, SLAVE_DATA_PORT);

	outb(ICW4, MASTER_DATA_PORT);
	outb(ICW4, SLAVE_DATA_PORT);

	outb(MASK_ALL, MASTER_DATA_PORT);     // maske all interrupts for slave and master
	outb(MASK_ALL, SLAVE_DATA_PORT);
}

/* Enable (unmask) the specified IRQ */

/* 
 * enable_irq
 * DESCRIPTION: change specific bit to 0
 * INPUT: irq_num 
 * 
 * 
 * 
 */
void
enable_irq(uint32_t irq_num)
{
	
	if(irq_num >= PORT_NUM)     /*slave interrupt enable*/
	{
		//tempstatus = inb(SLAVE_8259_PORT+1);
		//temp_status = (temp_status ^(1 << (irq_num -8)));

		//outb((inb(SLAVE_8259_PORT+1)^(1 << (irq_num -8))), SLAVE_8259_PORT+1);
		slave_mask = inb(SLAVE_DATA_PORT) & ~(1 << (irq_num-PORT_NUM));              // change one bit to 0
		outb(slave_mask, SLAVE_DATA_PORT);
	}
	else
	{
		//inb(MASTER_8259_PORT+1, temp_status);
		//temp_status = (temp_status ^ ~(1 << irq_num));
	
		//outb((inb(MASTER_8259_PORT+1)^ (1 << irq_num)), MASTER_8259_PORT+1);
		master_mask = inb(MASTER_DATA_PORT) & ~(1 << irq_num);
		outb(master_mask, MASTER_DATA_PORT);
	}
	
}

/* Disable (mask) the specified IRQ */
/* 
 * disable_irq
 * DESCRIPTION: change specific bit to 1
 * INPUT: irq_num 
 * 
 * 
 * 
 */
void
disable_irq(uint32_t irq_num)
{
	//uint8_t temp_status;
	if(irq_num >=PORT_NUM)
	{
		//inb(SLAVE_8259_PORT+1, temp_status);
		//temp_status = (temp_status | (1 << (irq_num -8)));
		outb((inb(SLAVE_DATA_PORT) | (1 << (irq_num -PORT_NUM))),SLAVE_DATA_PORT);     // change ont bit to onw
	}
	else
	{
		//inb(MASTER_8259_PORT+1, temp_status);
		//temp_status = (temp_status |(1 << irq_num));
		outb((inb(MASTER_DATA_PORT)|(1 << irq_num)) ,MASTER_DATA_PORT);
	}
}

/* Send end-of-interrupt signal for the specified IRQ */
/* 
 * send_eoi
 * DESCRIPTION: send eoi information to spcific irq
 * INPUT: irq_num 
 * 
 * 
 * 
 */
void
send_eoi(uint32_t irq_num)
{
	if( irq_num >= PORT_NUM)
	{
		outb(EOI|(irq_num-PORT_NUM), SLAVE_8259_PORT);                  // send eoi to port
		outb(EOI| 0x02, MASTER_8259_PORT); 
	}
	else
	{
		outb(EOI|irq_num, MASTER_8259_PORT);
	}
}

