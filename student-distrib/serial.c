/* SERIAL STUFF!

COM Port IO Port
COM1     0x3F8
COM2     0x2F8
COM3     0x3E8
COM4     0x2E8
*/

#include "serial.h"

#define COM1 0x3F8
#define COM2 0x2F8
//Don't use com higher than 2 as address is unreliable

static int8_t COM1_read_buf[256];   // array of chars we receive from COM1
static int32_t COM1_read_index;      // holds the location we start reading from when read gets called
static int8_t COM1_write_buf[256];   // array of chars we plan to send to COM1

static int8_t COM2_read_buf[256];   // array of chars we receive from COM2
static int32_t COM2_read_index;      // holds the location we start reading from when read gets called
static int8_t COM2_write_buf[256];   // array of chars we plan to send to COM2

void init_serial(int com, uint32_t baud_rate)
{
    uint32_t currPort;        // holds the current port value
    if(com == 1)
    {
        currPort = 0x3F8;
        enable_irq(4);
    }
    else if (com == 2)
    {
        currPort = 0x2F8;
        enable_irq(3);
    }
   else
        return;

   cli();
   outb(0x00, currPort + 1);    // Disable all interrupts
   outb(0x80, currPort + 3);    // Enable DLAB (set baud rate divisor)
   outb((115200/baud_rate), currPort + 0);    // Set divisor for baud rate     (lo byte)
   outb(0x00, currPort + 1);    //                                             (hi byte)
   outb(0x03, currPort + 3);    // 8 bits, no parity, one stop bit
   outb(0xC7, currPort + 2);    // Enable FIFO, clear them, with 14-byte threshold
   outb(0x01, currPort + 1);    // interrupt enabled for data available
   sti();
}

// checks whether there is a incoming message waiting for us in serial, depending on port
int serial_inbox(int com)
{
    uint32_t currPort;        // holds the current port value
    if(com == 1)
         currPort = 0x3F8;
    else if (com == 2)
         currPort = 0x2F8;
    else
         return -1;

    return inb(currPort + 5) & 1;
}

// checks if we can send out an outgoing message
int serial_outbox(int com)
{
    uint32_t currPort;        // holds the current port value
    if(com == 1)
         currPort = 0x3F8;
    else if (com == 2)
         currPort = 0x2F8;
    else
         return -1;

   return inb(currPort + 5) & 0x20;
}

// receives and returns an incoming byte waiting in the serial buffer (buffer size is 14 bits)
char serial_receive(int com)
{
    uint32_t currPort;        // holds the current port value
    if(com == 1)
         currPort = 0x3F8;
    else if (com == 2)
         currPort = 0x2F8;
    else
         return -1;

    while (serial_inbox(com) == 0);
    return inb(currPort);
}

void serial_send(int com, char message)
{
    uint32_t currPort;        // holds the current port value
    if(com == 1)
         currPort = 0x3F8;
    else if (com == 2)
         currPort = 0x2F8;
    else
         return;

   while (serial_outbox(com) == 0);
   outb(message, currPort);
}

/*
 * COM1_handler
 *   DESCRIPTION: reads from an IO port and stores the resulting value in a buffer
 *   INPUTS: none
 *   OUTPUTS: writes to a buffer
 *   RETURN VALUE: none
 *   SIDE EFFECTS: temporarily disables and reenables interrupts
 */
void COM1_handler()
{
    cli();
    int8_t serial_in;

    if(inb(COM1+5) & 0x82)      // check if there are errors. 0x82 is bit map of relevant error bits
    {
        outb(0xC7, COM1 + 2);
        return;
    }
    while(serial_inbox(1))      // check if there's anything we need to read in
    {
        serial_in = serial_receive(1);
        if(COM1_read_index == 255)  // check if we've reached the end of the buffer
            break;
        COM1_read_buf[COM1_read_index] = serial_in;    // reads in from defined PORT, places in buffer
        COM1_read_index++;   // increment buffer index
//        printf("%c", COM1_read_buf[COM1_read_index-1]);
    }
    send_eoi(4);    // send eoi for this port
    sti();
    return;
}

int32_t COM1_open (const uint8_t* filename)
{
    // reset the buffer and index
    int i;
    for(i=0; i<1024; i++)
    {
        COM1_read_buf[i] = 0;
        COM1_write_buf[i] = 0;
    }
    COM1_read_index = 0;

    outb(0xC7, COM1 + 2);   // resets FIFO buffer

    return 7;
}

int32_t COM1_read (int32_t fd, void* buf, int32_t nbytes)
{
    if(buf == NULL) // check for faulty buffer
        return -1;

    uint8_t* curr_buf;
    curr_buf = (uint8_t*)buf;

    int i;
    for(i = 0; i < nbytes; i++)     // copy everything over. not null-terminated
    {
        if(i == COM1_read_index)    // check if we've reached the end
            break;
        curr_buf[i] = COM1_read_buf[i];     // copies our own buffer into the parameter buffer
    }

    COM1_read_index = 0;    // reset index back to beginning
    return i;   // return number of bytes read into buffer
}

int32_t COM1_write (int32_t fd, const void* buf, int32_t nbytes)
{
    if(buf == NULL) // check for faulty buffer
        return -1;

    const uint8_t* curr_buf;
    curr_buf = (uint8_t*)buf;

    int i;
    for(i = 0; i < nbytes; i++)
    {
        if(i == 255)    // size of COM1_write_buf
            break;
        serial_send(1, curr_buf[i]); // send our array
    }

    return i;   // return number of bytes sent to the write buffer
}

int32_t COM1_close (int32_t fd)
{
    return 0;
}


/* COM2 STUFF BELOW */
/*
 * COM2_handler
 *   DESCRIPTION: reads from an IO port and stores the resulting value in a buffer
 *   INPUTS: none
 *   OUTPUTS: writes to a buffer
 *   RETURN VALUE: none
 *   SIDE EFFECTS: temporarily disables and reenables interrupts
 */
void COM2_handler()
{
    cli();
    int8_t serial_in;

    if(inb(COM2+5) & 0x82)      // check if there are errors. 0x82 is bit map of relevant error bits
    {
        outb(0xC7, COM2 + 2);
        return;
    }
    while(serial_inbox(2))      // check if there's anything we need to read in
    {
        serial_in = serial_receive(2);
        if(COM2_read_index == 255)  // check if we've reached the end of the buffer
            break;
        COM2_read_buf[COM2_read_index] = serial_in;    // reads in from defined PORT, places in buffer
        COM2_read_index++;   // increment buffer index
//        printf("%c", COM2_read_buf[COM2_read_index-1]);
    }
    send_eoi(3);    // send eoi for this port
    sti();
    return;
}

int32_t COM2_open (const uint8_t* filename)
{
    // reset the buffer and index
    int i;
    for(i=0; i<1024; i++)
    {
        COM2_read_buf[i] = 0;
        COM2_write_buf[i] = 0;
    }
    COM2_read_index = 0;

    outb(0xC7, COM2 + 2);   // resets FIFO buffer

    return 8;
}

int32_t COM2_read (int32_t fd, void* buf, int32_t nbytes)
{
    if(buf == NULL) // check for faulty buffer
        return -1;

    uint8_t* curr_buf;
    curr_buf = (uint8_t*)buf;

    int i;
    for(i = 0; i < nbytes; i++)     // copy everything over. not null-terminated
    {
        if(i == COM2_read_index)    // check if we've reached the end
            break;
        curr_buf[i] = COM2_read_buf[i];     // copies our own buffer into the parameter buffer
    }

    COM2_read_index = 0;    // reset index back to beginning
    return i;   // return number of bytes read into buffer
}

int32_t COM2_write (int32_t fd, const void* buf, int32_t nbytes)
{
    if(buf == NULL) // check for faulty buffer
        return -1;

    const uint8_t* curr_buf;
    curr_buf = (uint8_t*)buf;

    int i;
    for(i = 0; i < nbytes; i++)
    {
        if(i == 255)    // size of COM2_write_buf
            break;
        serial_send(2, curr_buf[i]); // send our array
    }

    return i;   // return number of bytes sent to the write buffer
}

int32_t COM2_close (int32_t fd)
{
    return 0;
}
