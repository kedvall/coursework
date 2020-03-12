
#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"

#define BUFSIZE 1024

int main ()
{
    uint32_t i, cnt, mouse_fd, color;
    uint8_t buf[BUFSIZE];
	int32_t read_buffer[2];
    int8_t write_buffer[2];
    write_buffer[0] = ' ';

/* 
    uint8_t clear_buf[25*80];
	uint8_t *vmem_base_addr;
	ece391_vidmap(&vmem_base_addr);

    ece391_memset(clear_buf, 0, 25*80);
	ece391_vidcpy(vmem_base_addr, clear_buf, 0);
	ece391_vidcpy(vmem_base_addr + 1, clear_buf, 0);
*/
    ece391_fdputs(1, (uint8_t*)"Choose a color: (0): Black, (1): Blue, (2): Green, (3): Teal, (4): Red, (5): Magenta, (6): Brown\n");
    if (-1 == (cnt = ece391_read(0, buf, BUFSIZE-1)) ) {
        ece391_fdputs(1, (uint8_t*)"Can't read the number from keyboard.\n");
     return 3;
    }
    buf[cnt] = '\0';

    switch (buf[0]) {
        case '0':
            color = 0;
            break;
        case '1':
            color = 1;
            break;
        case '2':
            color = 2;
            break;
        case '3':
            color = 3;
            break;
        case '4':
            color = 4;
            break;
        case '5':
            color = 5;
            break;
        case '6':
            color = 6;
            break;
    }
    write_buffer[1] = color;
	mouse_fd = ece391_open((uint8_t*)"MOUSE");
    /*for(i=0; i<25*80; i++)
    {
        *(uint8_t *)((uint8_t*)0xB8000 + (i << 1)) = ' ';
        *(uint8_t *)((uint8_t*)0xB8000 + (i << 1) + 1) = 0x7;
    }*/
	uint8_t key;
    while(1)
    {
    	ece391_read(11, &key, 1);
	    switch (key) {
	        case '0':
	            write_buffer[1] = 0;
	            break;
	        case '1':
	            write_buffer[1] = 1;
	            break;
	        case '2':
	            write_buffer[1] = 2;
	            break;
	        case '3':
	            write_buffer[1] = 3;
	            break;
	        case '4':
	            write_buffer[1] = 4;
	            break;
	        case '5':
	            write_buffer[1] = 5;
	            break;
	        case '6':
	            write_buffer[1] = 6;
	    }
        ece391_read(mouse_fd, read_buffer, 2);
        if(read_buffer[2]){
           colored_puts(&write_buffer[0], read_buffer[0], read_buffer[1], write_buffer[1], write_buffer[1]); 
        }
        ece391_write(mouse_fd, write_buffer, 0);
        //mouse_display();
    }
    return 0;
}

