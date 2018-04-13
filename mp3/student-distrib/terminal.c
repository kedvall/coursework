#include "terminal.h"

int32_t term_open(const uint8_t* filename)
{
  return 0;
}

/* Writes nbytes started at buf to the terminal */
int32_t term_write(int32_t fd, const void* buf, int32_t nbytes)
{
    int str_input;    // local flag to tell if we're printing a binary file
    const uint8_t* curr_buf;
    curr_buf = (uint8_t*)buf;

    if(buf == NULL)
      return 0;

  /* CHECK THE BUFFER FOR ELFISHNESS */
  if((curr_buf[0] != 0x7f) || (curr_buf[1] != 0x45) || (curr_buf[2] != 0x4C) || (curr_buf[3] != 0x46))
      str_input = 1;
  else
      str_input = 0;

  int i;
  for(i = 0; i < nbytes; i++)
  {
     if(curr_buf[i] == '\0')
     {
         //putc('0');
       if(str_input)
        break;
       else
        continue;
     }
    putc(curr_buf[i]);
  }
    return i;
}

int32_t term_read(int32_t fd, void* buf, int32_t nbytes)
{
  //terminal_t* a_term = &(term_array[active_term]);
  while(!mod_keys[4] || (curr_term != active_term) || (term_array[active_term].kb_redirect[0] > 0)) ;
  cli(); // do this or have enter be terminal dependent. YOU DECIDE.
  if(buf == NULL)
    return 0;

    uint8_t* curr_buf;
    curr_buf = (uint8_t*)buf;

  int32_t i;

  for(i = 0; i < nbytes; i++)
  {
    if(term_array[active_term].lineBuf[i] != '\0')
      curr_buf[i] = term_array[active_term].lineBuf[i];
    else
      break;
  }

  curr_buf[i] = '\0'; // set the last byte to NULL
  mod_keys[4] = 0;
  term_array[active_term].linePtr = 0;
  term_array[active_term].lineBuf[0] = '\0';
  sti();
  return i;
}

// woopsies now, we need this to close the term driver. but it just returns 0.
int32_t term_close(int32_t fd)
{
    return 0;
}
