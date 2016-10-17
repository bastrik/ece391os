/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */
#include "i8259.h"
#include "lib.h"
/* Interrupt masks to determine which interrupts
 * are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7 */
uint8_t slave_mask; /* IRQs 8-15 */
/* Initialize the 8259 PIC */
void
i8259_init(void)
{
	// initially mask everything
	master_mask = 0xFF;
	slave_mask = 0xFF;
	update_mask(master_mask, MASTER_8259_DATA);
    update_mask(slave_mask,  SLAVE_8259_DATA);
    // init master pic
    update_mask(ICW1, MASTER_8259_PORT);  
    update_mask(ICW2_MASTER, MASTER_8259_DATA);
    update_mask(ICW3_MASTER, MASTER_8259_DATA);     // slave on IRQ2
    update_mask(ICW4, MASTER_8259_DATA);            // 8086 mode
    // init slave pic
    update_mask(ICW1, SLAVE_8259_PORT); 
    update_mask(ICW2_SLAVE, SLAVE_8259_DATA);
    update_mask(ICW3_SLAVE, SLAVE_8259_DATA);
    update_mask(ICW4, SLAVE_8259_DATA);
}
/* Enable (unmask) the specified IRQ */
void
enable_irq(uint32_t irq_num)
{
    uint8_t word, port;
    // irq on master
	if (irq_num < 8)
	{		
    	port = MASTER_8259_DATA;
    }
    else
    {
		// first enable irq2 on master
        update_mask( (inb(MASTER_8259_DATA) & ~(1 << 2)), MASTER_8259_DATA);
        port = SLAVE_8259_DATA;
    	irq_num = irq_num - 8;
    }
	
    word = (inb(port) & ~(1 << irq_num));
    update_mask(word, port);
}
/* Disable (mask) the specified IRQ */
void
disable_irq(uint32_t irq_num)
{
    uint8_t word, port;
    // irq on master
	if (irq_num < 8) 
	{
    	port = MASTER_8259_DATA;
    }
    else
    {
        port = SLAVE_8259_DATA;
        irq_num = irq_num - 8;
    }
    word = (inb(port) | (1 << irq_num));
    update_mask(word, port);
}
/* Send end-of-interrupt signal for the specified IRQ */
void
send_eoi(uint32_t irq_num)
{
    // Send EOI to the slave pic.
	if (irq_num >= 8) 		
        outb( (EOI | irq_num -8), SLAVE_8259_PORT);
	// Send EOI to the master pic regardless.
    outb( (EOI | (irq_num)), MASTER_8259_PORT);
}
/* Helper function to send masks */
void update_mask(uint8_t word, uint8_t port)
{
	outb(word, port);
}
