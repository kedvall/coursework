#ifndef _PROCESS_INTERFACE_H
#define _PROCESS_INTERFACE_H

#include "types.h"
#include "global_includes.h"
#include "scheduler.h"

/* Global pointers to PCBs on kernel stack */
// pcb_t* processes[MAX_NUM_PROCESSES];         TERM_GLOBAL
//uint8_t process_flags[MAX_NUM_PROCESSES];    //TERM_GLOBAL

// extern int32_t cur_pid;                      //TERM_GLOBAL

extern pcb_t* get_cur_process();
extern file_array_t* get_cur_file(pcb_t* process, uint32_t fd);

#endif
