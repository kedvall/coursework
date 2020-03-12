#include "scheduler.h"

terminal_t term_array[3];   // array of our terminals
uint32_t video_array[3] = {BACKUP0, BACKUP1, BACKUP2};	//set this equal to vid mem backup addresses
uint8_t curr_term; // whichever next_term the scheduler is handling. not necessarily active
uint8_t active_term; // which terminal is active. all io (cursor, serial, keyboard) goes here. use this video memory

/*
 * scheduler_init
 *   DESCRIPTION: initializes scheduler
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:
 */
void scheduler_init()
{
    int i;
    curr_term = 0;
    active_term = 0;
/*    term_array[0].vid_mem_ptr = (uint32_t*)VID_MEM;
    term_array[1].vid_mem_ptr = (uint32_t*)video_array[1];
    term_array[2].vid_mem_ptr = (uint32_t*)video_array[2];*/

    // fill everything out with standard default values
    for(i=0; i<3; i++)
    {
        term_array[i].mouse_char = DEFAULT_MOUSE_CHAR;
        term_array[i].cur_pid = -1;
        term_array[i].screen_x = 0;
        term_array[i].screen_y = 0;
        term_array[i].linePtr = 0;
        term_array[i].lineBuf[0] = '\0';
        term_array[i].vid_mem_ptr = (uint8_t*)video_array[i];
    }

    // add in the special mouse attributes for each terminal
    term_array[0].mouse_attr = 0x3F;
    term_array[1].mouse_attr = 0x2F;
    term_array[2].mouse_attr = 0x4F;

    video_mem = (char*)(term_array[0].vid_mem_ptr);

	uint8_t program[] = "shell";
    execute(program);
}

/*
 * run_scheduler
 *   DESCRIPTION: called every 50ms, changes what the current process being run is by switching to the next
 *	 process' kernel stack
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:
 */
void run_scheduler()
{
    // clear interrupts first so that nasty pit doesn't interrupt us
    cli();

    // Increment curr_term
    int prev_term = curr_term;
    curr_term = (curr_term + 1) % 3;	//3 is the number of terminals we have
/*
    // change the video memory paging
    if(curr_term == active_term)
        second_page_table[0] = (VID_MEM) | USER_LEVEL_PAGE_FLAGS;
    else
        second_page_table[0] = ((unsigned int)term_array[curr_term].vid_mem_ptr) | USER_LEVEL_PAGE_FLAGS;
*/
    // copy over video memory from the active terminal's video buffer
    if(curr_term == active_term)
    {
    	//multiply by 2 to account for the character and attribute
        memcpy((void*)VID_MEM, (void*)video_array[active_term], NUM_COLS * NUM_ROWS * 2);

        // also draw the mouse pointer over the video memory
        mouse_display();

        // also show which terminal we're on
        if(active_term == 0)
        {
            *(uint8_t *)(VID_MEM + ((NUM_COLS*0 + NUM_COLS-1) << 1) + 1) = 0x03;     // teal text!
            *(uint8_t *)(VID_MEM + ((NUM_COLS*0 + NUM_COLS-1)<< 1)) = '1';
        }
        else if (active_term == 1)
        {
            *(uint8_t *)(VID_MEM + ((NUM_COLS*0 + NUM_COLS-1) << 1) + 1) = 0x02;     // green text!
            *(uint8_t *)(VID_MEM + ((NUM_COLS*0 + NUM_COLS-1) << 1)) = '2';
        }
        else if (active_term == 2)
        {
            *(uint8_t *)(VID_MEM + ((NUM_COLS*0 + NUM_COLS-1) << 1) + 1) = 0x04;     // red text!
            *(uint8_t *)(VID_MEM + ((NUM_COLS*0 + NUM_COLS-1)<< 1)) = '3';
        }
    }

    // Get new terminal ptr and set the new video_mem for lib.c functions
	terminal_t* next_term = &(term_array[curr_term]);
    video_mem = (char*)(next_term->vid_mem_ptr);

    uint32_t base;
    uint32_t stack;

    asm volatile ("movl %%ebp, %0" : "=g" (base));
    asm volatile ("movl %%esp, %0" : "=g" (stack));

    terminal_t* term = &(term_array[prev_term]);
    term->processes[term->cur_pid]->ebp = base;
    term->processes[term->cur_pid]->esp = stack;

    if(next_term->cur_pid == -1)
    {
        next_term->cur_pid = 0;
        uint8_t program[] = "shell";
        execute(program);
    }

    // Setup new paging
    /* We do this because the first 10 bits of 128MB in binary is 32. We are setting offset 32
	   in page directory to the start of the program because all programs are loaded at address 128MB */
    page_directory[32] = (PROGRAM_START_ADDRESS + (curr_term * 4 * PROGRAM_ALLOC_SIZE) + (next_term->cur_pid * PROGRAM_ALLOC_SIZE)) | PRGM_PD_4MB_K_RW_P;

    // Flush TLB by reloading cr3
    asm volatile("                  \n\
        movl %0, %%eax          \n\
        movl %%eax, %%cr3       \n\
        "
        :
        :"r"(page_directory)
        :"%eax"
        );

    // Switch esp/ebp to next process' kernel stack
    //4 because each terminal can run max 4 processes
    tss.esp0 = PROGRAM_START_ADDRESS - (curr_term * PROGRAM_KERN_STACK_SIZE * 4) - (PROGRAM_KERN_STACK_SIZE * next_term->cur_pid);

    second_page_table[0] = ((unsigned int)term_array[curr_term].vid_mem_ptr) | USER_LEVEL_PAGE_FLAGS;

	// Load all info for current process to appropriate place
	// Saving current ebp, esp of current process

    // here lies Kanyon's greatest idea for the team. The Swap. And the Pop.
    asm volatile ("movl %0, %%ebp" :: "g" (next_term->processes[next_term->cur_pid]->ebp));
    asm volatile ("movl %0, %%esp" :: "g" (next_term->processes[next_term->cur_pid]->esp));
}
