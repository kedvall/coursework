#include "rtc.h"
#include "types.h"
#include "global_includes.h"
#include "scheduler.h"
#include "i8259.h"
#include "lib.h"

//volatile int rtc_int_flag;
int rtc_testing;

/*
 * rtc_handler
 *   DESCRIPTION: Function to call when the RTC raises an interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Reads from Register C, saves and restore registers
 */
void rtc_handler()
{
    // call our test interrupt handler!
    // test_interrupts();

    // this reads from register C and then throws it away to ensure continued interrupts
    outb(REG_C, RTC_INDEX);
    inb(RTC_DATA);
    if(rtc_testing) // only if we've pressed ctrl-4 and not ctrl-5
    {
       putc('1');       // we print out '1' whenever we receieve an interrupt
    }
    uint32_t i;
    for(i=0; i<3; i++)
    {
        term_array[i].rtc_int_flag = 0; // set the flag back to zero for the rtc_read function
    }

    send_eoi(8); // send the EOI signal
}

/*
 * rtc_init
 *   DESCRIPTION: initialize the rtc and disable all interrupts (including NMI)
 *                temporarily
 *   INPUTS: none
 *   OUTPUTS: initializes the RTC!
 *   RETURN VALUE: none
 *   SIDE EFFECTS: disabled the all interrupts, including, NMI temporarily
 */
void rtc_init()
{
    cli();
    NMI_disable();      //disable NMI

    rtc_testing = 0;
    // turn on IRQ 8 and initialize rtc
    uint8_t prev;
    outb(REG_B, RTC_INDEX);
    prev=inb(RTC_DATA);	// read the current value of register B
    outb(REG_B, RTC_INDEX);		// set the index again (a read will reset the index to register D)
    outb(prev | SIX_BIT, RTC_DATA);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

    // now reenable all our interrupts
    NMI_enable();
    enable_irq(8);	// enable IR line for the RTC
    //interruptController(1);

    // also change the frequency to default lowest rate
    NMI_disable();
    int rate;
    rate &= REG_C;       //set 2Hz
    outb(REG_A, RTC_INDEX);     // set index to register A, disable NMI
    prev=inb(RTC_DATA); // get initial value of register A
    outb(REG_A, RTC_INDEX);     // reset index to A
    outb((prev & 0xF0) | rate, RTC_DATA); //write only our rate to A. Note, rate is the bottom 4 bits, so we do 0xF0

    uint32_t freq = 2; //Default should be six but there is no case for it?
    rtc_write(0, &freq, 4);

    //Enable interrupts
    NMI_enable();
    sti();

    return;
}

/*
 * rtc_open
 *   DESCRIPTION: We call this whenever we want to initialize the rtc driver with correct configuration
 *   INPUTS: filename. just call it with NULL
 *   OUTPUTS: none
 *   RETURN VALUE: 0. always.
 *   SIDE EFFECTS: Nothing much.
 */
int32_t rtc_open (const uint8_t* filename) {
    return 0;
}

/*
 * rtc_read
 *   DESCRIPTION: We call this whenever we want to delay and wait for the next interrupt from rtc
 *   INPUTS: just ignore these inputs.
 *   OUTPUTS: none
 *   RETURN VALUE: 0. always.
 *   SIDE EFFECTS: Delays until we receieve an interrupt
 */
int32_t rtc_read (int32_t fd, void* buf, int32_t nbytes) {
    sti();
    term_array[curr_term].rtc_int_flag = 1;
    //wait for interrupt
    while (term_array[curr_term].rtc_int_flag);   // wait for it to be turned back to 0
    return 0;
}
//have rtc int handler increment a counter and write to specific location on screen

/*
 * rtc_write
 *   DESCRIPTION: We call this when we want to set the frequency of the RTC driver
 *   INPUTS: don't worry about fd
 *           *buf- pass in the frequency through this buffer
 *           nbytes- number of bytes passed in through the buffer
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if fail
 *   SIDE EFFECTS: changes RTC driver frequency!
 */
int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes){
    //sys call should always accept only a 4 byte int specifiying the interrupt rate in Hz
    if (nbytes != 4){
        return -1;
    }

    //disable interrupts
    NMI_disable();
    int rate;
    int freq = *(int32_t*)buf;
    switch(freq) {
        case (2):   // case/switch statement for frequencies of every power of 2
            rate = 0x0F;    // we looked up these hex numbers
            break;
        case (4):
            rate = 0x0E;    // all looked up on the documentation for rtc!
            break;
        case (8):
            rate = 0x0D;    // yup.
            break;
        case (16):
            rate = 0x0C;
            break;
        case (32):
            rate = 0x0B;
            break;
        case (64):
            rate = 0x0A;
            break;
        case (128):
            rate = 0x09;
            break;
        case (256):
            rate = 0x08;
            break;
        case (512):
            rate = 0x07;
            break;
        case (MAX_FREQ):
            rate = 0x06;
            break;
        default:
            rate = 0x00;
    }

    //make sure rate is acceptable value
    if (rate != 0)
    {
        //Write rtc rate to rtc
        outb(REG_A, RTC_INDEX);     // set index to register A, disable NMI
        uint8_t prev=inb(RTC_DATA); // get initial value of register A
        outb(REG_A, RTC_INDEX);     // reset index to A
        outb((prev & 0xF0) | rate, RTC_DATA); //write only our rate to A. Note, rate is the bottom 4 bits.

        //Enable interrupts
        NMI_enable();
        return 0;
    }

    NMI_enable();
    return -1;
}

/*
 * rtc_close
 *   DESCRIPTION: We call this whenever we want to close the RTC driver
 *   INPUTS: fd- file descriptor. but we don't do anything with this
 *   OUTPUTS: none
 *   RETURN VALUE: 0. always.
 *   SIDE EFFECTS: Nothing. This function actually does nothing.
 */
int32_t rtc_close (int32_t fd)
{
    return 0;
}

// a function that waits a certain amount of time
void rtc_wait (int seconds, int freq)
{
    int ticks = seconds * freq;
    while(ticks)
    {
        rtc_read(0, 0, 0);
        ticks--;
    }
}

// FUNCTION GRAVEYARD
/*
void rtc_handler() {
    NMI_disable();

    outb(REG_C, RTC_INDEX);
    inb(RTC_DATA);
    rtc_int_flag=0;
    //clear the flag each time an interrupt is generated
    send_eoi(8);

    NMI_enable();
}*/
/*
void change_int_rate(uint8_t rate)
{
    uint32_t frequency;
	frequency =  32768 >> (rate-1);
 	rate &= 0x0F;			// rate must be above 2 and not over 15
	outb(REG_A, RTC_INDEX);		// set index to register A, disable NMI
	uint8_t prev=inb(RTC_DATA);	// get initial value of register A
	outb(REG_A, RTC_INDEX);		// reset index to A
	outb((prev & 0xF0 | rate, RTC_DATA); //write only our rate to A. Note, rate is the bottom 4 bits.
}
*/
