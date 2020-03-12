#include "mouse.h"

// variables for the mouse handler function
static uint8_t mouse_cycle=0;     // our packet is sent in 3 cycles of bytes. this shows that cycle
static uint8_t mouse_byte[3];    // holds the information mentioned above
int32_t mouse_x=0;         // x coordinate on screen, 0-80
int32_t mouse_y=0;         // y coordinate on screen, 0-25
static uint8_t previous_y;
static int32_t x=0;         // unrefined x position
static int32_t y=0;         // unrefined y position
static int32_t left_click=0;         // left mouse click
static int32_t right_click=0;         // right mouse click
static int32_t middle_click=0;         // middle mouse click
static int32_t t_x=0;         // x acceleration
static int32_t t_y=0;         // y acceleration

//changing mouse color based on which terminal we are in

// variables for the mouse cursor displayer function
int8_t saved_mouse_char;
static int32_t saved_y, saved_x;

//Mouse functions
void mouse_handler()
{
  mouse_byte[mouse_cycle] = inb(0x60);
  switch(mouse_cycle)
  {
    //packet sequence is off by 1 in QEMU?
    case 0:
      //mouse_byte[2]=inb(0x60);
      mouse_cycle++;
      break;
    case 1:
      //mouse_byte[0]=inb(0x60);
      //mouse_byte[0]=mouse_byte[0] | 0xFFFFFF00;
      //do things with bits
      mouse_cycle++;
      break;
    case 2:
      //mouse_byte[1]=inb(0x60);
      t_x = mouse_byte[2];
      t_y = mouse_byte[0];

      if(mouse_byte[1] & 0x10) t_x |= (-256);
      if(previous_y & 0x20) t_y |= (-256);
      if(mouse_byte[1] & 0x01) left_click = 1;
      if(mouse_byte[1] & 0x02) right_click = 1;
      if(mouse_byte[1] & 0x04) middle_click = 1;

      x += t_x;
      y -= t_y; //y is inverted

      //slowing it down so you can see it
      mouse_x = x >> 4;  // 4 and 5 are just good numbers to do this with
      mouse_y = y >> 5;

      //keeping the coordinates in bounds
      if(mouse_x < 0){
        mouse_x = 0;
        x = 0;
      }
      if(mouse_y < 0){
        mouse_y = 0;
        y = 0;
      }
      if(mouse_x > 79){
        mouse_x = 79;
        x = 1264;
      }
      if(mouse_y > 24){
        mouse_y = 24;
        y = 768;
      }
      mouse_cycle=0;
      previous_y = mouse_byte[1];
      break;
  }
//  mouse_display();    // no longer use this
  send_eoi(12);
}

inline void mouse_wait(uint8_t a_type) //unsigned char
{
  uint32_t _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

void write_mouse(uint8_t a_write) //unsigned char
{
  //Wait to be able to send a command
  //mouse_wait(1);
  //Tell the mouse we are sending a command
  outb(0xD4, 0x64);
  //Wait for the final part
  //mouse_wait(1);
  //Finally write
  outb(a_write, 0x60);
}

uint8_t read_mouse()
{
  //Get's response from mouse
  mouse_wait(0);
  return inb(0x60);
}

void mouse_init()
{
  uint8_t _status;  //unsigned char

  //Enable the auxiliary mouse device
  //mouse_wait(1);
  outb(0xA8, 0x64);

  //Enable the interrupts
  //mouse_wait(1);
  outb(0x20, 0x64);
  //mouse_wait(0);
  _status=(inb(0x60) | 2);
  //mouse_wait(1);
  outb(0x60, 0x64);
  //mouse_wait(1);
  outb(_status, 0x60);

  //Tell the mouse to use default settings
  write_mouse(0xF6);
  read_mouse();  //Acknowledge

  //Enable the mouse
  write_mouse(0xF4);
  read_mouse();  //Acknowledge

  saved_mouse_char = 0;
  saved_y = 0;
  saved_x = 0;

  enable_irq(12);
}

void mouse_display()
{
    /*
    if((saved_y != mouse_y) || (saved_x != mouse_x))    // check if our mouse has moved
    {
        colored_putc(saved_mouse_char, saved_x, saved_y, 0x0, 0x7);   // rewrite the saved characters
        saved_mouse_char = save_c(mouse_x, mouse_y);      // save the character in the new position
        saved_y = mouse_y;  // save the new coordinates
        saved_x = mouse_x;
        colored_putc(219, mouse_x, mouse_y, 0x0, 0x7);  // write the new mouse
    }
    */

    if(term_array[active_term].mouse_char != 0)
        *(uint8_t *)(VID_MEM + ((NUM_COLS*mouse_y + mouse_x) << 1)) = term_array[active_term].mouse_char;      // only if we want to write the char*/
    *(uint8_t *)(VID_MEM + ((NUM_COLS*mouse_y + mouse_x) << 1) + 1) = term_array[active_term].mouse_attr;     // writing the attribute

    return;
}

int32_t mouse_open (const uint8_t* filename){
  //fd 10
    return 10;
}

int32_t mouse_read (int32_t fd, void* buf, int32_t nbytes){
  //read the x and y position of mouse
  if(buf == NULL) // check for faulty buffer
        return -1;

    uint32_t* curr_buf;
    curr_buf = (uint32_t*)buf;

    curr_buf[0] = mouse_x;
    curr_buf[1] = mouse_y;
    curr_buf[2] = left_click;
    curr_buf[3] = right_click;
    curr_buf[4] = middle_click;

    return 0;
}
int32_t mouse_write (int32_t fd, const void* buf, int32_t nbytes){
  //write the character and attribute
  //buf holds 2 bytes = character and attribute
  if(buf == NULL) // check for faulty buffer
        return -1;

  const uint8_t* curr_buf;
  curr_buf = (uint8_t*)buf;

  term_array[0].mouse_char = curr_buf[0];
  term_array[0].mouse_attr = curr_buf[1];
  return 0;
}
int32_t mouse_close (int32_t fd){
  return 0;
}
