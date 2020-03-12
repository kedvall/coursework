#include "keyboard.h"

#define ctrl_demo 0 // turn the ctrl-num stuff on or off
void term_switch();

int32_t player_pos = 0;
terminal_t* a_term;

// modifier keys are stored in an array as follows: [capslock, shift, ctrl, alt, enter]
uint8_t mod_keys[5];

// KB redirect array!
// if kb_redirect[0] is 0, then we are print keyboard output to screen, and save it into the buffer
// if kb_redirect[0] is 1, then we are not printing keyboard output, but still save it into the buffer
// if kb_redirect[0] is 2, then we are not printing keyboard output, nor saving it in the buffer
// we will always save the last printed character into kb_redirect[1]
// uint8_t kb_redirect[1];

// Store keys we care about that aren't ascii
/* Array layout:
    0 - 11 : F1 - F12
    12 : ESC
    13 : Tab
    14 : Left arrow
    15 : Right arrow
    16 : Up arrow
    17 : Down arrow
    18 : Delete */
uint8_t non_ascii[NUM_NON_ASCII] = {0};

int32_t keyboard_open(const int8_t* filename)
{
    return 11;
}

int32_t keyboard_close(int32_t fd)
{
    return 0;
}

int32_t keyboard_read(int32_t fd, void* buf, int32_t nbytes)
{
    if(nbytes != 1)
        return -1;
    if(SCHEDULER_ON){
      a_term = &(term_array[active_term]);
    }
    else{
      a_term = &(term_array[0]);
    }
    
    uint8_t* key_press = (uint8_t*) buf;
    *key_press = a_term->kb_redirect[1];

    a_term->kb_redirect[1] = 0;

    return 0;
}

int32_t keyboard_write(int32_t fd, const void* buf, int32_t nbytes)
{
    if(nbytes != 1)
        return -1;

    if(SCHEDULER_ON){
      a_term = &(term_array[active_term]);
    }
    else{
      a_term = &(term_array[0]);
    }

    uint8_t* key_config = (uint8_t*) buf;
    a_term->kb_redirect[0] = *key_config;

    return 0;
}

/* Scanline-to-Key array */
static uint8_t kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
    '9', '0', '-', '=', '\b',	/* Backspace */
    '\t',			/* Tab */
    'q', 'w', 'e', 'r',	/* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
    '\'', '`',   0,		/* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
    'm', ',', '.', '/',   0,				/* Right shift */
    '*',
    0,	/* Alt */
    ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
    '-',
    0,	// Left Arrow 75
    0,
    0,	// Right Arrow 77
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

static uint8_t kbdus_shift[128] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
    '(', ')', '_', '+', '\b',	/* Backspace */
    '\t',			/* Tab */
    'Q', 'W', 'E', 'R',	/* 19 */
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* Enter key */
    0,			/* 29   - Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
    '\"', '~',   0,		/* Left shift */
    '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
    'M', '<', '>', '?',     // everything else we don't care about
};

/*
 * keyboard_init
 *   DESCRIPTION: initializes keyboard
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:
 */
void keyboard_init()
{
    //terminal_t* a_term = &(term_array[active_term]);
    int i;
    for(i=0; i<5; i++)
        mod_keys[i] = 0;

    enable_irq(1);

    term_array[active_term].kb_redirect[0] = 0;
}

/*
 * keyboardHandler
 *   DESCRIPTION: acquires the scancode from port 0x60 and prints out the
 *   the ASCII character
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints out to video memory
 */
void keyboardHandler()
{
  //terminal_t* a_term = &(term_array[active_term]);
  // Interrupts are now disabled at this point
  uint8_t code;
  code = inb(0x60); // read the char from the keyboard

  if(!(code & 0x80))
  {
    //code = kbdus[code];
    switch(code)   // CHECKING KEY MODIFIER PRESSED
    {
      case 29: // ctrl
        mod_keys[2] = 1;
        break;
      case 54: // Right shift
        mod_keys[1] = 1;
        non_ascii[(active_term + 1) % 3] = 1;
        term_switch();
        break;
      case 42: // left shift
        mod_keys[1] = 1;
        break;
      case 58: // caps lock
        mod_keys[0] = (mod_keys[0] == 1) ? 0 : 1;
        break;
      case 56:  // alt pressed
        mod_keys[3] = 1;
        break;
      case 28: // Enter
        if(term_array[active_term].kb_redirect[0] == 0)
        {
            buffered_putc('\n');
            term_array[active_term].lineBuf[(term_array[active_term].linePtr)++] = '\n';
            term_array[active_term].lineBuf[term_array[active_term].linePtr] = '\0';
            mod_keys[4] = 1;
        }
        else if (term_array[active_term].kb_redirect[0] == 205)
        {
            mod_keys[4] = 1;
        }
        break;
      case 14: // backspace
        if(term_array[active_term].linePtr != 0)
        {
            term_array[active_term].lineBuf[--(term_array[active_term].linePtr)] = '\0';
            buffered_puts(term_array[active_term].lineBuf, term_array[active_term].linePtr);
        }
        break;

      case 59: //F1
        non_ascii[0] = 1;
        if (mod_keys[3]) // Hot key with alt (Term 1)
          term_switch();
        break;
      case 60: //F2
        non_ascii[1] = 1;
        if (mod_keys[3]) // Hot key with alt (Term 2)
          term_switch();
        break;
      case 61: //F3
        non_ascii[2] = 1;
        if (mod_keys[3]) // Hot key with alt (Term 3)
          term_switch();
        break;
      case 62: //F4
        non_ascii[3] = 1;
        break;
      case 63: //F5
        non_ascii[4] = 1;
        break;
      case 64: //F6
        non_ascii[5] = 1;
        break;
      case 65: //F7
        non_ascii[6] = 1;
        break;
      case 66: //F8
        non_ascii[7] = 1;
        break;
      case 67: //F9
        non_ascii[8] = 1;
        break;
      case 68: //F10
        non_ascii[9] = 1;
        break;
      case 87: //F11
        non_ascii[10] = 1;
        break;
      case 88: //F12
        non_ascii[11] = 1;
        break;

      case 1: //ESC
        non_ascii[12] = 1;
        break;
      case 83: //Delete
        non_ascii[12] = 1;
        break;
      case 75: //Left arrow
        non_ascii[14] = 1;
        term_array[0].kb_redirect[1] = 200;
        break;
      case 77: //Right arrow
        non_ascii[15] = 1;
        term_array[0].kb_redirect[1] = 201;
        break;
      case 72: //Up arrow
        non_ascii[12] = 1;
        term_array[0].kb_redirect[1] = 202;
        break;
      case 80: //Down arrow
        non_ascii[12] = 1;
        term_array[0].kb_redirect[1] = 203;
        break;

      //FIND ACTUAL KEYCODES FOR THESE!
      case 251: //Tab
        non_ascii[12] = 1;
        break;

      default:      // if we're doing a regular ascii key
        if(mod_keys[1])
            code = kbdus_shift[code];
        else
            code = kbdus[code];
        if(!code)
          break;
        /* Ctrl + l to clear screen */
        if((code == 'l') && mod_keys[2])
        {
            rtc_testing = 0;  //clear flag so 1 doesn't print
            clearShell();
        }
        else if(term_array[active_term].linePtr < 127)      // if our buffer isn't full
        {
          if(mod_keys[0])    // if pressed, flip the capitalization around
          {
            if((code >= 65) && (code <= 90))
                code += 32;
            else if ((code >= 97) && (code <= 122))
                code -= 32;
          }

          term_array[active_term].kb_redirect[1] = code;
          if(term_array[active_term].kb_redirect[0] == 0)
          {
              term_array[active_term].lineBuf[(term_array[active_term].linePtr)++] = code;
              buffered_putc(code);
          }
          else if(term_array[active_term].kb_redirect[0] == 1)
          {
              term_array[active_term].lineBuf[(term_array[active_term].linePtr)++] = code;
          }
        }
        break;
    }
  }
  else
  {
    /* In here we check to see whether ctrl/shift have been released */
    switch (code)
    {
      case 170: // left shift release
        mod_keys[1] = 0;
        break;
      case 182: // right shift release
        mod_keys[1] = 0;
        break;
      case 157: // ctrl release
        mod_keys[2] = 0;
        break;
      case 184:     // alt released
        mod_keys[3] = 0;
        break;
      default:
        break;
    }
  }

  send_eoi(1); // send the EOI signal
}

/*
 * term_switch
 *   DESCRIPTION: Changes which one is the active terminal based on which key is pressed.
 *   Copies the current video memory to the correct backup buffer and copies the new active
 *   terminal's backup buffer to the actual video memory
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints out to video memory
 */
void term_switch()
{
    if (!SCHEDULER_ON)
      return;

    cli();
    uint32_t i;

    //terminal_t* a_term = &(term_array[active_term]);
    term_array[active_term].vid_mem_ptr = (uint8_t*)video_array[active_term]; //Switch video pointer to backup
/*
    //Copy over current vid mem to backup
    memcpy(term_array[active_term].vid_mem_ptr, (void*)VID_MEM, NUM_COLS * NUM_ROWS * 2);
*/
    if (non_ascii[0])
        active_term = 0;
    else if (non_ascii[1])
        active_term = 1;
    else if (non_ascii[2])
        active_term = 2;

    //a_term = &(term_array[active_term]);
//    term_array[active_term].vid_mem_ptr = (uint32_t*)VID_MEM; //Switch video pointer to real video memory

    //Copy from backup to actual
    memcpy((void*)VID_MEM, (void*)video_array[active_term], NUM_COLS * NUM_ROWS * 2);

    for (i = 0; i < NUM_NON_ASCII; i++) //Clear Non-ascii flags
        non_ascii[i] = 0;

    update_cursor(1);
}
