#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"

int main ()
{
    int i = 0;
    int end_i = 0;
    int j;
    int k = 1;

    int32_t cnt;
    uint8_t buf[256];

    int8_t read_buffer[256];
    uint8_t write_buffer[256];

    write_buffer[0] = 254;
    ece391_write(7, write_buffer, 1);


    while (1)
    {
        ece391_fdputs (1, (uint8_t*)"Enter some text to send!\n");

        if (-1 == (cnt = ece391_read (0, buf, 255))) {
            ece391_fdputs (1, (uint8_t*)"Can't read name from keyboard.\n");
            return 3;
        }

        buf[cnt] = '\0';
        ece391_fdputs (1, (uint8_t*)"Sending: ");
        ece391_fdputs (1, buf);

        ece391_strcpy(write_buffer, (uint8_t*)"Husky");
        ece391_open((uint8_t*)"COM1"); // open up our com1 port
<<<<<<< HEAD

        ece391
        while(1)
=======
        
        // this is just a giant delay
        for(j=0; j<4200000;j++)
            k = k*42;
        k = 1;

        // this reads in stuff from the buffer
        end_i = ece391_read(7, read_buffer, 256);
        if(end_i)
>>>>>>> 79745740dc5e7f467c190cdd5b7dd57c9fea91e4
        {
            for(i=0; i < end_i; i++)
            {
                ece391_fdputs (1, (uint8_t*)read_buffer);
            }
            ece391_fdputs (1, (uint8_t*)"\n");
        }

        // this writes something out to the buffer
        ece391_write(7, write_buffer, 5);
        }
    return 0;
}
