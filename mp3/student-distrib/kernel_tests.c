/***************************************************************************************
*   Use this file to write tests for the kernel!                                       *
*   Tests should be easy to enable / disable, see below for format                     *
*   Tests should also return a string indicating the name of the function that failed, *
*       or 0 if ALL tests passed                                                       *
***************************************************************************************/
#include "kernel_tests.h"

/************************************************************************
*   Add a global enable / disable switch here for each test function    *
************************************************************************/
#define en_paging 0
#define en_idt 0
#define en_demo 0
#define en_directory 0
#define en_rtc 0
#define en_serial 0
#define en_video 0
#define en_mouse 1
#define en_combined 0

/***********************************************************************
*   Add testing functions here!                                        *
***********************************************************************/

int demo()
{
    int freq = 2;
    terminal_t* a_term = &(term_array[active_term]);
    a_term->kb_redirect[0] = 2; // redirect keyboard output to the demo
    while(1)
    {
        sti();
        if((a_term->kb_redirect[1] == '1') && mod_keys[2])
        {
            a_term->kb_redirect[1] = 0;
            //list files by name
            rtc_testing = 0;  //clear flag so 1 doesn't print
            clear();  //clears the screen
            demo_filesys(1); // call function with code 1 pressed
        }
        else if((a_term->kb_redirect[1] == '2') && mod_keys[2])
        {
            a_term->kb_redirect[1] = 0;
            //list files by name
            rtc_testing = 0;  //clear flag so 1 doesn't print
            clear();  //clears the screen
            demo_filesys(2); // call function with code 2 pressed
        }
        else if((a_term->kb_redirect[1] == '3') && mod_keys[2])
        {
            a_term->kb_redirect[1] = 0;
            rtc_testing = 0;  //clear flag so 1 doesn't print
            clear();  //clears the screen
            demo_filesys(3); // call this function with code 3 pressed
        }
        else if((a_term->kb_redirect[1] == '4') && mod_keys[2])
        {
            a_term->kb_redirect[1] = 0;
            //rtc frequency changes every time
            clear();  //clears the screen
            rtc_testing = 1;
            rtc_write(0, &freq, 4); //change frequency in rtc_write
            freq *= 2;              //double frequency each time
            if(freq > MAX_FREQ){
              freq = 2;
            }
        }
        else if((a_term->kb_redirect[1] == '5') && mod_keys[2])
        {
            a_term->kb_redirect[1] = 0;
            rtc_testing = 0;  //clear flag so 1 doesn't print
            clear();
        }
        else if(mod_keys[1] && mod_keys[2] && mod_keys[3])  // when we want to end
        {
            break;
        }
    }
    return 0;
}

/*
 * idt_test
 *   DESCRIPTION: Tests that IDT has been correctly initialized
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 on success, -1 if ANY tests fail
 */
int idt_test()
{
    // DIVIDE BY ZERO EXCEPTION TEST (Uncomment to run, prevents compilation warning)
    //printf("Div by 0 test: %d", (8 / 0)); // Divide by zero

    //Getting here means we've failed the test
    printf("Failed divide by 0 test");
    return -1;
}

/* directory read test
*/
int directory_read_test()
{
  uint8_t filename[33];

  int ret;
  ret = directory_read(0, filename, 32);
  do {
    filename[32] = '\0';
    cli();
    printf("%s\n", filename);
    sti();
    ret = directory_read(0, filename, 32);
  } while(ret);

  return 0;
}

/*
 * paging_test
 *   DESCRIPTION: Tests various paging aspects
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 on success, -1 if ANY tests fail
 */
int paging_test()
{
    char run_pagefault = 0; //Set this to run pagefault test

    uint32_t* ptr = NULL; //Pointer for our tests

    if (run_pagefault)
    {
        // PAGE FAULT EXCEPTION TEST
        printf("NULL test: %d\n", *ptr);

        //Getting here means we've failed the test
        printf("Failed dereferencing NULL test");
        return -1;
    }


    // PROPER ADDRESS DEREFERENCING TEST
    ptr = (uint32_t*) 0x400002;
    printf("Addr 0x01 Test:\n");
    printf("Deref 1: %u\n", *ptr);
    printf("Deref 2: %u\n", *ptr);

    if (*ptr != 0x400002)
    {
        printf("Failed actual address dereferencing");
        return -1;
    }

    return 0;
}

int rtc_test(){
    printf("open_rtc: %d\n", rtc_open(NULL));
    printf("close_rtc: %d\n", rtc_close(0));
    int freq = 32;
    rtc_write(0, &freq, 4);

    int rtc_i=0;
    for (rtc_i=0;rtc_i<20;rtc_i++){
        rtc_read(0,NULL,0);
        putc('1');
    }
    freq = 2;
    rtc_write(0, &freq, 4);
    for (rtc_i=0;rtc_i<20;rtc_i++){
        rtc_read(0,NULL,0);
        putc('0');
    }
    return 0;
}


int serial_test()
{
    printf("Starting serial test\n");
    int8_t read_buffer[256];
    int8_t write_buffer[256] = "Husky";
    int i = 0;
    int end_i = 0;
    int j;
    int k = 1;
    //COM1_open(NULL);
    while(1)
    {
        sti();
        // this is just a giant delay
        for(j=0; j<4200000;j++)
            k = k*42;
        k = 1;

        // this reads in stuff from the buffer
        end_i = COM1_read(0, read_buffer, 256);
        if(end_i)
        {
            for(i=0; i < end_i; i++)
            {
                printf("%c", read_buffer[i]);
            }
            printf("\n");
        }

        // this writes something out to the buffer
        COM1_write(0, write_buffer, 5);
    }
    return 0;
}

int mouse_test(){
    /*
    while(1)
    {
        printf("\nx: %d",mouse_x);
        printf("y: %d",mouse_y);
    }
    return 0;
    */
    printf("Starting mouse test\n");
    int32_t read_buffer[2];
    int8_t write_buffer[2];
    write_buffer[0] = '5';
    write_buffer[1] = 0xf2;
    mouse_open(NULL);
    while(1)
    {
        mouse_read(0, read_buffer, 2);
        printf("x: %d", read_buffer[0]);
        printf("y: %d\n", read_buffer[1]);
        mouse_write(0, write_buffer, 0);
        //mouse_display();
    }
    return 0;
}

int video_test()
{
    int i, j, k, l;
    k=0;
    l=0;
    int8_t text_buffer[256] = "BG:  C: ";
    while(1)
    {
        sti();
        for(i=0; i<25; i++)
        {
            text_buffer[3] = l+48;
            for(j=0; j<20; j++)
            {
                clear();
                text_buffer[7] = k+48;
                colored_puts(text_buffer, 4*j, i, i, j);
                while(!mod_keys[4]);
                mod_keys[4] = 0;
                k++;
                if(k>9)
                    k=0;
            }
            k=0;
            l++;
            if(l>9)
                l=0;
        }
    }
    return 0;
}

int combined_testing()
{
    while(1)
    {
      clear();
      colored_puts("#", mouse_x, mouse_y, 0x0, 0x7);
    }
}

/*

 * run_ktests
 *   DESCRIPTION: Runs tests specified above on the kernel
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
int run_ktests()
{
    printf("Starting kernel tests!\n");

    int ret_val = 0;

    // Tests here!
    if (en_demo)
        ret_val |= demo();

    if (en_idt)
        ret_val |= idt_test();

    if (en_paging)
        ret_val |= paging_test();

    if (en_rtc)
        ret_val |= rtc_test();

    if(en_directory)
        ret_val |= directory_read_test();

    if (en_serial)
        ret_val |= serial_test();

    if (en_video)
        ret_val |= video_test();

    if (en_mouse)
        ret_val |= mouse_test();

    if (en_combined)
        ret_val |= combined_testing();

    return ret_val;
}

// we can run this function when we want to run on a single terminal for some reason.
void single_term()
{
    curr_term = 0;
    active_term = 0;
    term_array[0].mouse_char = DEFAULT_MOUSE_CHAR;
    term_array[0].mouse_attr = DEFAULT_MOUSE_ATTR;
    term_array[0].cur_pid = 0;
    term_array[0].screen_x = 0;
    term_array[0].screen_y = 0;
    term_array[0].linePtr = 0;
    term_array[0].lineBuf[0] = '\0';
    term_array[0].vid_mem_ptr = (uint8_t*)video_array[0];
    video_mem = (char*)(term_array[0].vid_mem_ptr);

    sti();
}
