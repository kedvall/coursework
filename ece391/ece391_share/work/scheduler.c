#include "scheduler.h"

terminal_t term_array[3];   // array of our terminals
uint32_t video_array[3] = {0xB9000, 0xBA000, 0xBB000};	//set this equal to vid mem backup addresses
uint8_t curr_term; // whichever next_term the scheduler is handling. not necessarily active
uint8_t active_term; // which terminal is active. all io (cursor, serial, keyboard) goes here. use this video memory

void scheduler_init()
{
    curr_term = 0;
    active_term = 0;
    term_array[0].cur_pid = -1;
    term_array[1].cur_pid = -1;
    term_array[2].cur_pid = -1;
    term_array[0].vid_mem_ptr = (uint32_t*)VID_MEM;
    term_array[1].vid_mem_ptr = (uint32_t*)video_array[1];
    term_array[2].vid_mem_ptr = (uint32_t*)video_array[2];
	uint8_t program[] = "shell";
    execute(program);
}


void run_scheduler()
{
    // Increment curr_term
    int prev_term = curr_term;
    curr_term = (curr_term + 1) % 3;

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
    tss.esp0 = PROGRAM_START_ADDRESS - (curr_term * PROGRAM_KERN_STACK_SIZE * 4) - (PROGRAM_KERN_STACK_SIZE * next_term->cur_pid);
    //Comment
	// Load all info for current process to appropriate place
	// Saving current ebp, esp of current process

    // here lies Kanyon's greatest idea for the team. The Swap. And the Pop.
    asm volatile ("movl %0, %%ebp" :: "g" (next_term->processes[next_term->cur_pid]->ebp));
    asm volatile ("movl %0, %%esp" :: "g" (next_term->processes[next_term->cur_pid]->esp));
}
