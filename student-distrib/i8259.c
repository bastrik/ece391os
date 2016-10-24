/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts
 * are enabled and disabled */
uint8_t master_mask; 
uint8_t slave_mask; 

#define MASK_ALL         0xFF
#define PORT_NUM         8
#define MASTER_DATA_PORT 0x21
#define SLAVE_DATA_PORT  0xA1

/* Initialize the 8259 PIC */

 /*
 * i8259_init
 *   DESCRIPTION: Sends control words to appropriate port
 *                  and masks all interrupts.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Enables IRQ on PIC
 *                  and initializes variables
 */
void
i8259_init(void)
{
    /* send control words to both PICs */
    outb(ICW1,MASTER_8259_PORT);                  
    outb(ICW1,SLAVE_8259_PORT);

    outb(ICW2_MASTER, MASTER_DATA_PORT);
    outb(ICW2_SLAVE, SLAVE_DATA_PORT);

    outb(ICW3_MASTER,MASTER_DATA_PORT);
    outb(ICW3_SLAVE, SLAVE_DATA_PORT);

    outb(ICW4, MASTER_DATA_PORT);
    outb(ICW4, SLAVE_DATA_PORT);

    /* masks all interrupts */
    outb(MASK_ALL, MASTER_DATA_PORT);
    outb(MASK_ALL, SLAVE_DATA_PORT);
}

/* Enable (unmask) the specified IRQ */

 /*
 * enable_irq
 *   DESCRIPTION: Unmasks the specified IRQ
 *   INPUTS: irq_num -- IRQ number to be enabled
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Enables IRQ on PIC
 *                  and initializes variables
 */
void
enable_irq(uint32_t irq_num)
{
    /* Enable Master PIC interrupt */
    if(irq_num < PORT_NUM)     
    {
        master_mask = inb(MASTER_DATA_PORT) & ~(1 << irq_num);
        outb(master_mask, MASTER_DATA_PORT);
    }
    /* Enable Slave PIC interrupt */
    else
    {
        slave_mask = inb(SLAVE_DATA_PORT) & ~(1 << (irq_num-PORT_NUM));
        outb(slave_mask, SLAVE_DATA_PORT);
    }
    
}

/* Disable (mask) the specified IRQ */

 /*
 * disable_irq
 *   DESCRIPTION: Masks the specified IRQ
 *   INPUTS: irq_num -- IRQ number to be disabled
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Disables IRQ on PIC
 *                  and initializes variables
 */
void
disable_irq(uint32_t irq_num)
{
    /* Disable Master PIC interrupt */
    if(irq_num <PORT_NUM)
    {
        outb((inb(MASTER_DATA_PORT)|(1 << irq_num)) ,MASTER_DATA_PORT);
    }
    /* Disable Slave PIC interrupt */
    else
    {
        outb((inb(SLAVE_DATA_PORT) | (1 << (irq_num -PORT_NUM))),SLAVE_DATA_PORT);     // change one bit to 1
    }
}

/* Send end-of-interrupt signal for the specified IRQ */

 /*
 * send_eoi
 *   DESCRIPTION: Send end-of-interrupt signal for the specified IRQ
 *   INPUTS: irq_num -- IRQ number to send eoi
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: eoi signal is sent to PIC
 */
void
send_eoi(uint32_t irq_num)
{
    /* send eoi to Master PIC */
    if( irq_num < PORT_NUM)
    {
        outb(EOI|irq_num, MASTER_8259_PORT);
    }
    /* send eoi to Slave PIC */
    else
    {
        outb(EOI|(irq_num-PORT_NUM), SLAVE_8259_PORT);                  // send eoi to port
        outb(EOI| 0x02, MASTER_8259_PORT);
    }
}
