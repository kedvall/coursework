#include "process_interface.h"

pcb_t* get_cur_process()
{
    terminal_t* term = &(term_array[curr_term]);
	return term->processes[term->cur_pid];
}

file_array_t* get_cur_file(pcb_t* process, uint32_t fd)
{
	return &(process->f_descriptors[fd]);
}
