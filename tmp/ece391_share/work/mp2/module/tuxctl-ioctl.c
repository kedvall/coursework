/* tuxctl-ioctl.c
 *
 * Driver (skeleton) for the mp2 tuxcontrollers for ECE391 at UIUC.
 *
 * Mark Murphy 2006
 * Andrew Ofisher 2007
 * Steve Lumetta 12-13 Sep 2009
 * Puskar Naha 2013
 */

#include <asm/current.h>
#include <asm/uaccess.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/miscdevice.h>
#include <linux/kdev_t.h>
#include <linux/tty.h>
#include <linux/spinlock.h>

#include "tuxctl-ld.h"
#include "tuxctl-ioctl.h"
#include "mtcp.h"

#define debug(str, ...) \
    printk(KERN_DEBUG "%s: " str, __FUNCTION__, ## __VA_ARGS__)

// Locally defined functions
int init_tux(struct tty_struct* tty);
void update_cached_buttons(unsigned char byte_1, unsigned char byte_2);
int get_tux_buttons(struct tty_struct* tty, unsigned long arg);
int set_tux_leds(struct tty_struct* tty, unsigned long arg);

// Variable to store the state of the TUX
static unsigned char tux_locked;
// Variable to store the TUX's LED states in case of reset
static unsigned long led_cache;
// Array to store the state of the TUX's butttons. For ease of use it is 8 bytes
// Packaged as: RIGHT, DOWN, LEFT, UP, C, B, A, START
static unsigned char button_cache[8];


/************************ Protocol Implementation *************************/

/* tuxctl_handle_packet()
 * IMPORTANT : Read the header for tuxctl_ldisc_data_callback() in 
 * tuxctl-ld.c. It calls this function, so all warnings there apply 
 * here as well.
 */
void tuxctl_handle_packet (struct tty_struct* tty, unsigned char* packet)
{
    unsigned a, b, c;

    a = packet[0]; /* Avoid printk() sign extending the 8-bit */
    b = packet[1]; /* values when printing them. */
    c = packet[2];

    switch (a) {
        case MTCP_ACK :
            //This is received when we're done with a command, use it to "unlock" the TUX for more commands
            tux_locked = 0;
            return;
        case MTCP_RESET :
            //This is received when the Reset button is pushed causing the board to reset
            init_tux(tty); //TUX needs to be reinitialized
            set_tux_leds(tty, led_cache); //Restore the cached LEDs
            tux_locked = 0;
            return;
        case MTCP_BIOC_EVENT :
            //if (EN_DEBUG) printk("BIOC Event!\n");
            //A button was updated, update our corresponding cached values
            update_cached_buttons(b, c);
            return;
    }

    return;
}

/******** IMPORTANT NOTE: READ THIS BEFORE IMPLEMENTING THE IOCTLS ************
 *                                                                            *
 * The ioctls should not spend any time waiting for responses to the commands *
 * they send to the controller. The data is sent over the serial line at      *
 * 9600 BAUD. At this rate, a byte takes approximately 1 millisecond to       *
 * transmit; this means that there will be about 9 milliseconds between       *
 * the time you request that the low-level serial driver send the             *
 * 6-byte SET_LEDS packet and the time the 3-byte ACK packet finishes         *
 * arriving. This is far too long a time for a system call to take. The       *
 * ioctls should return immediately with success if their parameters are      *
 * valid.                                                                     *
 *                                                                            *
 ******************************************************************************/
/* tuxctl_ioctl()
 *   DESCRIPTION: Dispatcher for various TUX functions
 *   INPUTS: A tty buffer and an arguement to be passed, as well as what command to execute
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Calls a TUX function
 */
int 
tuxctl_ioctl (struct tty_struct* tty, struct file* file, 
          unsigned cmd, unsigned long arg)
{
    switch (cmd) {
    case TUX_INIT:
        init_tux(tty);
        break;
    case TUX_BUTTONS:
        get_tux_buttons(tty, arg);
        break;
    case TUX_SET_LED:
        set_tux_leds(tty, arg);
        break;
    case TUX_LED_ACK:
    case TUX_LED_REQUEST:
    case TUX_READ_LED:
    default:
        return -EINVAL;
    }

    return 0;
}


/*
 * init_tux
 *   DESCRIPTION: Sets up the TUX controller
 *   INPUTS: A tty buffer
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */  
int init_tux(struct tty_struct* tty)
{
    //Make a buffer for sending commands to the TUX
    unsigned char buf[2];

    buf[0] = MTCP_BIOC_ON; //Turn on button interrupt-on-change, generate an interrupt whenever button is pressed
    
    buf[1] = MTCP_LED_USR; //Set LED mode 

    //Send this to the TUX
    if (tuxctl_ldisc_put(tty, buf, 2) != 0)
        return -EINVAL;

    return 0;
}

/*
 * update_cached_buttons
 *   DESCRIPTION: Triggered on BTN interrupt, updates stored button values
 *   INPUTS: Two bytes holding values of the buttons
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Changes values of the buttons internally
 */  
void update_cached_buttons(unsigned char byte_1, unsigned char byte_2)
{
    int i;
    unsigned int bit_mask = 0x01; //Mask to grab lowest bit

    //Unpackage the passed in values and write them into our button cache
    button_cache[7] = (byte_2 >> 3) & bit_mask;
    button_cache[6] = (byte_2 >> 2) & bit_mask;
    button_cache[5] = (byte_2 >> 1) & bit_mask;
    button_cache[4] = (byte_2) & bit_mask;
    button_cache[3] = (byte_1 >> 3) & bit_mask;
    button_cache[2] = (byte_1 >> 2) & bit_mask;
    button_cache[1] = (byte_1 >> 1) & bit_mask;
    button_cache[0] = (byte_1) & bit_mask;

    for (i = 0; i < 8; i++)
        button_cache[i] = !button_cache[i]; //Invert all the bits for active high
}


/*
 * get_tux_buttons
 *   DESCRIPTION: Called by user, retreives the stored button values
 *   INPUTS: arg - buffer to pass button values into
 *   OUTPUTS: none
 *   RETURN VALUE: A byte containing the button values
 *   SIDE EFFECTS: none
 */  
int get_tux_buttons(struct tty_struct* tty, unsigned long arg)
{
    unsigned char compressed_buttons = 0x00; //Variable to hold a compressed version of the button cache

    //Compress the individual button bytes into a single byte representing all the buttons
    compressed_buttons |= (button_cache[7] << 7);
    compressed_buttons |= (button_cache[6] << 6);
    compressed_buttons |= (button_cache[5] << 5);
    compressed_buttons |= (button_cache[4] << 4);
    compressed_buttons |= (button_cache[3] << 3);
    compressed_buttons |= (button_cache[2] << 2);
    compressed_buttons |= (button_cache[1] << 1);
    compressed_buttons |= (button_cache[0] << 0);

    //Copy from user space to kernel space
    if (copy_to_user((void*)arg, (void*)&compressed_buttons, sizeof(unsigned long)) > 0)
        return -EINVAL; //Return ERROR if not all the bits could be copied (pointer was invalid)

    return 0;
}


/*
 * set_tux_leds
 *   DESCRIPTION: Set the LEDs on the TUX
 *   INPUTS: arg - Encoded data for which LEDs, decimals, to turn on, and numbers to display
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */  
int set_tux_leds(struct tty_struct* tty, unsigned long arg)
{
    /*  arg format: (number is what byte it is)
     *   ________________________________________________________________________________
     *  | - | Decimals to turn on | - | LEDs to turn on | HEX 4 | HEX 3 | HEX 2 | HEX 1 |
     *  + 4 +------ 4 ------------+ 3 +------ 3 --------+-- 2 --+-- 2 --+-- 1 --+-- 1 --+ */

    unsigned char buf[6]; //Buffer to hold arguements to pass to TUX
    unsigned long hex_values[4]; //Hex values passed in (to display)
    unsigned long led_seg[4]; //Which LEDs to turn on
    unsigned long dec_seg[4]; //Which decimals to turn on
    unsigned int cur_mask;
    unsigned int byte_mask = 0xFF; //Mask to grab whole byte
    unsigned int dec_set_mask = 0x0010000; //Mask to set
    int i, temp; //Temp counter and data swapping vars
    int en = 0; //Tracks how many LEDs are enabeled 

    // Update the cached value in case of TUX reset
    led_cache = arg;

    // Check if the TUX is locked (already in use)
    if (tux_locked)
        return -EINVAL;
    tux_locked = 1; //Indicate the TUX is now busy

    // Get hex values from arg
    cur_mask = 0x000F;
    for(i = 0; i < 4; i++)
    {
        hex_values[i] = (cur_mask & arg);
        hex_values[i] >>= 4 * i;
        cur_mask <<= 4; //Move bitmask
    }

    // Get led segment data
    cur_mask = 0x0010000;
    for(i = 0; i < 4; i++)
    {
        led_seg[i] = (cur_mask & arg);
        led_seg[i] >>= (i + 16);
        cur_mask <<= 1; //Move bitmask
    }

    // Get decimal segment data
    cur_mask = 0x01000000;
    for(i=0; i<4; i++)
    {
        dec_seg[i] = (cur_mask & arg);
        dec_seg[i] >>= (i+24);
        cur_mask <<= 1; //Move bitmask
    }

    // Build buffer to send data to TUX
    buf[0] = MTCP_LED_SET;

    // Determine which LEDs to turn on
    cur_mask = 0x00F0000;
    byte_mask = (cur_mask & arg);
    byte_mask >>= 16;
    buf[1] = byte_mask; //Send it to buffer

    //Add decimal data
    en = 2;
    for(i = 0; i < 4; i++)
    {
        // Check is LED segment is on (enabeled)
        if (led_seg[i])
        {
            // Get segment values
            temp = hex_values[i];
            buf[en] = LED_HEX_VAL[temp];

            if (dec_seg[i])
                buf[en] |= 0x10; //Add to buffer
            en++; //Increases bytes to send
        }
        else
        {
            buf[1] = 0x0F;
            buf[en] = 0x00;
            en++;
        }
    }

    //Write the data to the TUX
    tuxctl_ldisc_put(tty, buf, en);

    return 0;
}
