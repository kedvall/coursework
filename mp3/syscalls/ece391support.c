#include <stdint.h>

#include "ece391support.h"
#include "ece391syscall.h"

#define NUM_COLS 80
#define NUM_ROWS 25

// this writes a colored, null-terminated string. we use coordinates, but if any of them is -1, we default to the cursor location
// returns number of bytes written
int32_t colored_puts(int8_t* s, int x_coord, int y_coord, uint8_t backg_color, uint8_t text_color)
{
    // our local screen coord ints
    int local_screen_y;
    int local_screen_x;
    register int32_t index = 0;

    if((x_coord < 0) || (y_coord < 0) || (x_coord > NUM_COLS) || (y_coord > NUM_ROWS))  // are we using default cursor location?
    {
    	while(s[index] != '\0')
        {
    		colored_putc(s[index], -1, -1, backg_color, text_color);
    		index++;
    	}
    }
    else    // else, we're using the coordinates we passed in
    {
        local_screen_y = y_coord;
        local_screen_x = x_coord;
        while(s[index] != '\0')
        {
            colored_putc(s[index], local_screen_x, local_screen_y, backg_color, text_color);
            local_screen_x++;
            if(local_screen_x == NUM_COLS)
            {
              local_screen_x = 0;
              local_screen_y++;
            }
            index++;
        }
    }
	return index;
}

// this writes a colored character. we use coordinates, but if any of them is -1, we default to the cursor location
void colored_putc(int8_t c, int x_coord, int y_coord, uint8_t backg_color, uint8_t text_color)
{
    // fetch our base address!
    uint8_t *vmem_base_addr;
    ece391_vidmap(&vmem_base_addr);

    // merge the backg color and the text_color together
    backg_color = backg_color & 0x07;
    text_color = text_color & 0x0F;
    uint8_t attribute = (backg_color << 4) + text_color;

    // check if any of our parameter coordinates are invalid, so we use default cursor location
    if((x_coord < 0) || (y_coord < 0) || (x_coord > NUM_COLS) || (y_coord > NUM_ROWS))
    {
        return;
    }
    else    // otherwise, we use passed in coordinates
    {
        if (c != '\n' && c != '\r')
        {
            *(uint8_t *)(vmem_base_addr + ((NUM_COLS*y_coord + x_coord) << 1)) = c;
            *(uint8_t *)(vmem_base_addr + ((NUM_COLS*y_coord + x_coord) << 1) + 1) = attribute;
        }
    }
    return;
}

// this returns the current character at the coordinates
int8_t save_c(int x_coord, int y_coord)
{
    // fetch our base address!
    uint8_t *vmem_base_addr;
    ece391_vidmap(&vmem_base_addr);

    // check if any of our parameter coordinates are invalid
    if((x_coord < 0) || (y_coord < 0) || (x_coord > NUM_COLS) || (y_coord > NUM_ROWS))
        return -1;
    else    // otherwise, we return the char at the coordinates
        return *(uint8_t *)(vmem_base_addr + ((NUM_COLS*y_coord + x_coord) << 1));
}

uint32_t ece391_strlen(const uint8_t* s)
{
    uint32_t len;

    for (len = 0; '\0' != *s; s++, len++);
    return len;
}

void ece391_strcpy(uint8_t* dst, const uint8_t* src)
{
    while ('\0' != (*dst++ = *src++));
}

void ece391_fdputs(int32_t fd, const uint8_t* s)
{
    (void)ece391_write (fd, s, ece391_strlen(s));
}

int32_t ece391_strcmp(const uint8_t* s1, const uint8_t* s2)
{
    while (*s1 == *s2) {
        if (*s1 == '\0')
            return 0;
        s1++;
        s2++;
    }
    return ((int32_t)*s1) - ((int32_t)*s2);
}

int32_t ece391_strncmp(const uint8_t* s1, const uint8_t* s2, uint32_t n)
{
    if (0 == n)
        return 0;
    while (*s1 == *s2) {
        if (*s1 == '\0' || --n == 0)
        return 0;
    s1++;
    s2++;
    }
    return ((int32_t)*s1) - ((int32_t)*s2);
}

/* Convert a number to its ASCII representation, with base "radix" */
uint8_t* ece391_itoa(uint32_t value, uint8_t* buf, int32_t radix)
{
        static int8_t lookup[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        uint8_t *newbuf = buf;
        int32_t i;
        uint32_t newval = value;

        /* Special case for zero */
        if(value == 0) {
                buf[0]='0';
                buf[1]='\0';
                return buf;
        }

        /* Go through the number one place value at a time, and add the
         * correct digit to "newbuf".  We actually add characters to the
         * ASCII string from lowest place value to highest, which is the
         * opposite of how the number should be printed.  We'll reverse the
         * characters later. */
        while (newval > 0) {
                i = newval % radix;
                *newbuf = lookup[i];
                newbuf++;
                newval /= radix;
        }

        /* Add a terminating NULL */
        *newbuf = '\0';

        /* Reverse the string and return */
        return ece391_strrev(buf);
}

/* In-place string reversal */
uint8_t* ece391_strrev(uint8_t* s)
{
    register uint8_t tmp;
    register int32_t beg = 0;
    register int32_t end = ece391_strlen(s) - 1;

    if (end <= 0) {
        return s;
    }

    while (beg < end) {
        tmp = s[end];
        s[end] = s[beg];
        s[beg] = tmp;
        beg++;
        end--;
   }

   return s;
}


int32_t ece391_vidcpy(void* dest, const void* src, uint32_t start)
{
    int32_t i, j;
    int32_t pos = start;
    char* d = (char*)dest;
    char* s = (char*)src;

    for (i = 0; i < SCREEN_H; i++)
    {
        for (j = 0; j < SCREEN_W; j++, pos++)
            d[(j << 1) + (i * SCREEN_W * 2)] = s[pos];

        pos += (MAP_WIDTH - SCREEN_W);
    }

    return 0;
}


uint8_t* mp1_set_video_mode (void)
{
    if(ece391_vidmap(&vmem_base_addr) == -1) {
        return NULL;
    } else {
        return vmem_base_addr;
    }
}


void ece391_memset(void* memory, char c, int n)
{
    char* mem = (char*)memory;
    int i;
    for(i=0; i<n; i++) {
        mem[i] = c;
    }
}
