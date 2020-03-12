/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"
#include "rtc.h"

/* Interrupt masks to determine which interrupts
 * are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7 */
uint8_t slave_mask; /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void
i8259_init(void)
{
	master_mask = ALL_ONES;
	slave_mask = ALL_ONES;

	outb(master_mask, MASTER_8259_PORT + 1);
	outb(slave_mask, SLAVE_8259_PORT + 1);

	outb(ICW1, MASTER_8259_PORT);
	outb(ICW2_MASTER, MASTER_8259_PORT+1);
	outb(ICW3_MASTER, MASTER_8259_PORT+1);
	outb(ICW4, MASTER_8259_PORT+1);

	outb(ICW1, SLAVE_8259_PORT);
	outb(ICW2_SLAVE, SLAVE_8259_PORT+1);
	outb(ICW3_SLAVE, SLAVE_8259_PORT+1);
	outb(ICW4, SLAVE_8259_PORT+1);
}

/* Enable (unmask) the specified IRQ */
void
enable_irq(uint32_t irq_num)
{
	if(irq_num < 0 || irq_num > UPPER_RANGE){
		return;	//out of range
	}

	if(irq_num & SLAVE_BIT){
		//slave
		//11111110 before
		uint8_t mask = ~(1 << (irq_num - SLAVE_BIT));
		slave_mask &= mask;
		outb(slave_mask, SLAVE_8259_PORT+1);
		enable_irq(SLAVE_IRQ);
		return;
	}
	uint8_t mask = ~(1 << irq_num);
	master_mask &= mask;
	outb(master_mask, MASTER_8259_PORT+1);
	return;
}

/* Disable (mask) the specified IRQ */
void
disable_irq(uint32_t irq_num)
{

	if(irq_num < 0 || irq_num > UPPER_RANGE){
		return;	//out of range
	}

	if(irq_num & SLAVE_BIT){
		//slave
		uint8_t mask = 1 << (irq_num - SLAVE_BIT);
		slave_mask |= mask;
		outb(slave_mask, SLAVE_8259_PORT+1);
		return;
	}

	//master
	//00000001 before
	uint8_t mask = 1 << irq_num;
	master_mask |= mask;
	outb(master_mask, MASTER_8259_PORT+1);

}

/* Send end-of-interrupt signal for the specified IRQ */
void
send_eoi(uint32_t irq_num)
{
	if(irq_num & SLAVE_BIT){
		//slave
		outb((EOI | (irq_num - SLAVE_BIT)), SLAVE_8259_PORT);
		outb((EOI | SLAVE_IRQ), MASTER_8259_PORT);
	}
	else{
		//master
		outb((EOI | irq_num), MASTER_8259_PORT);
	}
}


// function to enable non-maskable interrupt, using ports
// INPUT: none
// OUTPUT: none
void NMI_enable(void)
{
   outb(RTC_INDEX, inb(0x70)&0x7F); // enables NMI interrupt
}


// function to disable non-maskable interrupt, using ports
// INPUT: none
// OUTPUT: none
void NMI_disable(void)
{
   outb(RTC_INDEX, inb(0x70)|0x80);
}


// function to disable/enable interrupt mask, depending on what you pass in
// INPUT: int enabler- pass in 1 to enable interrupts, 0 to disable
// OUTPUT: none
void interruptController(int enabler)
{
	static uint8_t saved_master; // save IRQ 0-7
	static uint8_t saved_slave; // save IRQ 8-15
	if(enabler)
	{
		master_mask = (saved_master & master_mask);	// return the mask to saved values
		slave_mask = (saved_slave & master_mask);
	}
	else
	{
		saved_master = master_mask;
		saved_slave = slave_mask;
		master_mask = ALL_ONES;
		slave_mask = ALL_ONES;
	}
	return;
}
