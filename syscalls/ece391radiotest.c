#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"

int8_t saved_y;
int8_t saved_x;
int8_t saved_mouse_char;

int main ()
{
    int i = 0;
    int end_i = 0;
    int j;
    int k = 1;

    int8_t read_buffer[256];
    int8_t write_buffer[256];

    uint8_t code_buffer[256];

    ece391_fdputs (1, (uint8_t*)"Starting user-level ardutester!\n");
//    ece391_strcpy(write_buffer, (uint8_t*)"Husky");
    ece391_open((uint8_t*)"COM1"); // open up our com1 port

    code_buffer[0] = 253;
    ece391_write(7, code_buffer, 1);

    saved_mouse_char = 0;
    saved_y = 0;
    saved_x = 0;

    while(1)
    {
        /*
        // this is just a giant delay
        for(j=0; j<4200000;j++)
            k = k*42;
        k = 1;
*/
        // this reads in stuff from the buffer
        ece391_read(7, read_buffer, 3);

        if(read_buffer[0] > 79)
            read_buffer[0] = 79;
        else if (read_buffer[0] < 0)
            read_buffer[0] = 0;
        if(read_buffer[1] > 24)
            read_buffer[1] = 24;
        else if (read_buffer[1] < 0)
            read_buffer[0] = 0;

/*
            // disregard crazy values
        if(((read_buffer[0] - saved_x) > 20) || ((saved_x - read_buffer[0]) > 20))
        {
            read_buffer[0] = saved_x;
        }
        if(((read_buffer[1] - saved_y) > 5) || ((saved_y - read_buffer[1]) > 5))
        {
            read_buffer[1] = saved_y;
        }
*/
        if((saved_y != read_buffer[1]) || (saved_x != read_buffer[0]))    // check if our mouse has moved
        {
            colored_putc(saved_mouse_char, saved_x, saved_y, 0x0, 0x7);   // rewrite the saved characters
            saved_mouse_char = save_c(read_buffer[0], read_buffer[1]);      // save the character in the new position
            saved_y = read_buffer[1];  // save the new coordinates
            saved_x = read_buffer[0];
            colored_putc(219, read_buffer[0], read_buffer[1], 0x0, 0x7);  // write the new mouse
        }
    }
    return 0;
}
