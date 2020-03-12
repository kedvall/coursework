#include "sys_calls.h"

/* Variable used to figure out where to load program in memory */
static int32_t num_processes = 0;
// int32_t cur_pid = 0; // GLOBAL TERM

static int32_t error_func();

/*
 * execute
 *   DESCRIPTION: Sets up a program for execution, then executes it
 *   INPUTS: Command to execute
 *   OUTPUTS: none
 *   RETURN VALUE: -1 if error, 0 if okay
 *   SIDE EFFECTS: Runs a program
 */
int32_t execute(const uint8_t* command)
{
	cli();
    /*
    pcb_t* current_pid;
    current_pid = get_cur_process();
    if(current_pid)
    {
        printf("current term is %d\n", curr_term);
        printf("active term is %d\n", active_term);
    }
    */
	/*----------EXTRACT THE COMMAND----------*/
    if (num_processes == MAX_NUM_PROCESSES)
    {
        printf("Process limit reached: ");
        return -1;
    }

	int i = 0, j = 0;
	int arg_start, command_start = 0, command_end = 0;

	// I decree the max size of the command to be 128 bytes
	uint8_t cmd[128];

	//dealing with leading white spaces
	while(command[command_start]==' ')
	 	command_start++;//Move command start to first non-space char

	command_end = command_start; //Start the end of the command at first non-space char

	while( (command[command_end]!=' ') && (command[command_end] != '\0') && ((command_end-command_start) < 32))    // must be less than 32 so we don't buffer overflow
		command_end++;

	for(i = 0; i < (command_end - command_start); i++)
		cmd[i] = command[i + command_start];

	cmd[i] = '\0'; // This line replaced the newline char with null

	/* cmd buffer now holds the parsed command with no white spaces (or newlines) */

	/*----------EXTRACT THE ARGUMENT----------*/
	j = 0;
	arg_start = command_end;

	while(command[i++]==' ')
	{
		arg_start++;
	}

	while(command[arg_start] != '\0' && command[arg_start] != ' ')
	{
		term_array[curr_term].args[j++] = command[arg_start++];
	}

	// args[arg_start-j] = '\0'; // <-- what does this line do? It looks wrong....
	term_array[curr_term].args[j] = '\0';

	if (strncmp("exit", (int8_t*)cmd, 4) == 0)
	{
		halt_wrap(0); //go to halt
	}

	/*----------Grab the file if it exists----------*/
	// THIS is the civilized way of reading a file
	uint8_t file_header[FILE_HEADER_SIZE];

	dentry_t file;

	if (!read_dentry_by_name(cmd, &file))
		read_data(get_inode_ptr(file.inode), 0, file_header, FILE_HEADER_SIZE);
	else
		return -1;

	/* CHECK THE FILE FOR ELFISHNESS */
	if((file_header[0] != 0x7f) || (file_header[1] != 0x45) || (file_header[2] != 0x4C) || (file_header[3] != 0x46))
		return -1;

	/*----------Acquire address of first instruction----------*/
	/* I flipped the bytes around because, as you very well know, this is little endian. */
	uint32_t first_instruction = (file_header[27] << 24) |
	                             (file_header[26] << 16) |
	                             (file_header[25] << 8)  |
	                             (file_header[24]);

    terminal_t* term = &(term_array[curr_term]);    // save address of whatever our current terminal is

    /*----------GRAB THE NEW PROCESS ID!----------------------*/
    for(i = 0; i < MAX_NUM_PROCESSES; i++)
    {
        if (!(term->process_flags[i]))  // this is the way the cool kids iterate through process flags
        {
            term->process_flags[i] = 1;
            break;
        }
    }
    if(i == MAX_NUM_PROCESSES)  // if we can fit no more new processes
        return -1;
    int32_t child_pid = i;      // save iterated i as the child pid

	/* Add/Modify entry in page table for the new process */
	page_directory[32] = (PROGRAM_START_ADDRESS + (curr_term * 4 * PROGRAM_ALLOC_SIZE) + (child_pid * PROGRAM_ALLOC_SIZE)) | PRGM_PD_4MB_K_RW_P;
	/* We do this because the first 10 bits of 128MB in binary is 32. We are setting offset 32
	   in page directory to the start of the program because all programs are loaded at address 128MB */

	/* Flush the TLB by reloading cr3 */
	asm volatile("                  \n\
			movl %0, %%eax          \n\
			movl %%eax, %%cr3       \n\
			"
			:
			:"r"(page_directory)
			:"%eax"
			);

	/* Now let's copy the file contents into physical memory */
	read_data(get_inode_ptr(file.inode), 0, (uint8_t*)VIRT_PROGRAM_START, PROGRAM_ALLOC_SIZE);

	/*-----------Add a new PCB-----------*/
	term->processes[child_pid] = (pcb_t*)(PROGRAM_START_ADDRESS - PROGRAM_KERN_STACK_SIZE * (child_pid + 1) - (curr_term * PROGRAM_KERN_STACK_SIZE * 4));
	if(num_processes == 0) // if we're starting something for the first time
    {
		term->processes[child_pid]->parent_pcb = term->processes[0];      // our parent is ourself!
        term->processes[child_pid]->pid = 0;
    }
	else
    {
		term->processes[child_pid]->parent_pcb = term->processes[term->cur_pid]; // <-- this will work for everything!
        term->processes[child_pid]->pid = child_pid;
    }
    term->cur_pid = child_pid;    // *sniff* this is so beautiful. the child is growing up to be his own parent.

	/* Initialize the flags all to 0 so we know none of the files are being used */
	for(i = 0; i < MAX_NUM_FILES; i++)
	{
		term->processes[term->cur_pid]->f_descriptors[i].flags = 0x0000;
	}

	/* Setup stdin */
	term->processes[term->cur_pid]->f_descriptors[0].f_ops.f_open = &term_open;
	term->processes[term->cur_pid]->f_descriptors[0].f_ops.f_write = &error_func;
	term->processes[term->cur_pid]->f_descriptors[0].f_ops.f_read = &term_read;
	term->processes[term->cur_pid]->f_descriptors[0].f_ops.f_close = &term_close;

	term->processes[term->cur_pid]->f_descriptors[0].inode = 0x00000000;
	term->processes[term->cur_pid]->f_descriptors[0].f_pos = 0;
	term->processes[term->cur_pid]->f_descriptors[0].flags = 0x0001;

	/* Setup stdout*/
	term->processes[term->cur_pid]->f_descriptors[1].f_ops.f_open = &term_open;
	term->processes[term->cur_pid]->f_descriptors[1].f_ops.f_write = &term_write;
	term->processes[term->cur_pid]->f_descriptors[1].f_ops.f_read = &error_func;
	term->processes[term->cur_pid]->f_descriptors[1].f_ops.f_close = &term_close;

	term->processes[term->cur_pid]->f_descriptors[1].inode = 0x00000000;
	term->processes[term->cur_pid]->f_descriptors[1].f_pos = 0;
	term->processes[term->cur_pid]->f_descriptors[1].flags = 0x0001;

	/* Modify the TSS */
	tss.esp0 = PROGRAM_START_ADDRESS - (curr_term * PROGRAM_KERN_STACK_SIZE * 4) - (PROGRAM_KERN_STACK_SIZE * term->cur_pid);
	num_processes++;

    // also modify KB_redirect based off the program we're opening. FISH and PINGPONG block keyboard input
    int8_t fish_strcmp[] = "fish";
    int8_t pinpong_strcmp[] = "pingpong";

    // now we go through and see if the name we're opening matches one of our custom drivers
    if(!(strncmp((int8_t*)cmd, fish_strcmp, 4)))   // check if we're opening fish!
    {
        term_array[active_term].kb_redirect[0] = 2; // block all keyboard output
    }
    else if(!(strncmp((int8_t*)cmd, pinpong_strcmp, 8)))  // check if we're opening pingpong!
    {
        term_array[active_term].kb_redirect[0] = 2; // block all keyboard output
    }

	sys_call_execute(first_instruction);

	return 0;
}

/*
 * error_func
 *   DESCRIPTION: Function mapped to invalid option in function pointer set
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: -1
 *   SIDE EFFECTS: none
 */
static int32_t error_func()
{
	return -1;
}

/*
 * read
 *   DESCRIPTION: Reads from a entry in fd array using function pointer
 *   INPUTS: fd to read from, buffer to fill, number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: -1
 *   SIDE EFFECTS: none
 */
int32_t read (int32_t fd, void* buf, int32_t nbytes)
{
    if(fd == 7)
        return COM1_read(fd, buf, nbytes);
    else if(fd == 8)
        return COM2_read(fd, buf, nbytes);
    else if(fd == 9)
        return sound_read(fd, buf, nbytes);      //sound
    else if (fd == 10)
        return mouse_read(fd, buf, nbytes);      // mouse
    else if (fd == 11)
    	return keyboard_read(fd, buf, nbytes);   // Keyboard
    else if (fd == 12)
    	return gui_read(fd, buf, nbytes);

	if((fd < 0) || (fd > 7))
		return -1;

	file_array_t* file = get_cur_file(get_cur_process(), fd);

	if (!file->flags)
		return -1;

	int32_t bytes_read = (*(file->f_ops.f_read))(fd, buf, nbytes);
	file->f_pos += bytes_read;
	return bytes_read;
}

/*
 * Write
 *   DESCRIPTION: Writes to a file
 *   INPUTS: fd to write to, buffer containing text, number of bytes to write
 *   OUTPUTS: none
 *   RETURN VALUE: number of bytes written
 *   SIDE EFFECTS: May set RTC
 */
int32_t write (int32_t fd, const void* buf, int32_t nbytes)
{
    if(fd == 7)
        return COM1_write(fd, buf, nbytes);
    else if(fd == 8)
        return COM2_write(fd, buf, nbytes);
    else if(fd == 9)
        return sound_write(fd, buf, nbytes);      //sound
    else if (fd == 10)
        return mouse_write(fd, buf, nbytes);     // mouse
    else if (fd == 11)
    	return keyboard_write(fd, buf, nbytes);  // Keyboard
    else if (fd == 12)
    	return gui_write(fd, buf, nbytes);

	if((fd < 0) || (fd > 7))
		return -1;

	file_array_t* file = get_cur_file(get_cur_process(), fd);

	if (!file->flags)
		return -1;

	return (*(file->f_ops.f_write))(fd, buf, nbytes);
}

/*
 * open
 *   DESCRIPTION: Opens a file / directory / device
 *   INPUTS: filename to open
 *   OUTPUTS: none
 *   RETURN VALUE: file descriptor
 *   SIDE EFFECTS: Sets up function pointers for file type
 */
int32_t open (const int8_t* filename)
{
	/* 1. Look for open fd in array---
	   2. set flag in use---
	   3. setup for table inode--, ---pos (call corresponding open)
	   4. return fd */
    int8_t com1_strcmp[] = "COM1";          // 7
    int8_t com2_strcmp[] = "COM2";          // 8
    int8_t mouse_strcmp[] = "MOUSE";        // 10
    int8_t keyboard_strcmp[] = "KEYBOARD";  // 11
    int8_t player_strcmp[] = "PLAYER";  	// 12
	//int8_t timer_strcmp[] = "TIMER" // 13

    // now we go through and see if the name we're opening matches one of our custom drivers
    if(!(strncmp(filename, com1_strcmp, 4)))   // check if we're opening a COM1 driver
        return COM1_open(NULL);
    else if (!(strncmp(filename, com2_strcmp, 4)))  // check if we're opening a COM2 driver
        return COM2_open(NULL);
    else if (!(strncmp(filename, mouse_strcmp, 5))) // check if we're opening the mouse driver
        return mouse_open(NULL);
    else if (!(strncmp(filename, keyboard_strcmp, 8)))
    	return keyboard_open(NULL);
    else if (!(strncmp(filename, player_strcmp, 6)))
    	return gui_open(NULL);

	dentry_t file;
	if(file_load((uint8_t*)filename, &file) < 0)
		return -1;

	pcb_t* cur_process = get_cur_process();

	/* Search for an empty entry in the file array */
	uint32_t i;
	for(i = 0; i < MAX_NUM_FILES; i++)
	{
		if(cur_process->f_descriptors[i].flags == 0x0000)
		{
			cur_process->f_descriptors[i].flags = 0x0001; // LSB set means in use
			break;
		} /*else {
			if (cur_process->f_descriptors[i].inode == get_inode_ptr(file.inode))
				return -1;
				//check this out
		}*/
	}

	/* If i is the max number of files, we've exhausted the fds and must gtfo */
	if(i == MAX_NUM_FILES)
		return -1;

	/* Acquire inode pointer */
	cur_process->f_descriptors[i].inode = get_inode_ptr(file.inode);

	switch(file.file_type)
	{
		case 0:
			cur_process->f_descriptors[i].f_ops.f_open = &rtc_open;
			cur_process->f_descriptors[i].f_ops.f_read = &rtc_read;
			cur_process->f_descriptors[i].f_ops.f_write = &rtc_write;
			cur_process->f_descriptors[i].f_ops.f_close = &rtc_close;
			rtc_open((uint8_t*)filename);
			break;
		case 1:
			cur_process->f_descriptors[i].f_ops.f_open = &directory_open;
			cur_process->f_descriptors[i].f_ops.f_read = &directory_read;
			cur_process->f_descriptors[i].f_ops.f_write = &directory_write;
			cur_process->f_descriptors[i].f_ops.f_close = &directory_close;
			directory_open((uint8_t*)filename);
			break;
		case 2:
			cur_process->f_descriptors[i].f_ops.f_open = &file_open;
			cur_process->f_descriptors[i].f_ops.f_read = &file_read;
			cur_process->f_descriptors[i].f_ops.f_write = &file_write;
			cur_process->f_descriptors[i].f_ops.f_close = &file_close;
			file_open((uint8_t*)filename);
			break;
		default:
			return -1;
	}

	cur_process->f_descriptors[i].f_pos = 0;
	return i;
}

/*
 * close
 *   DESCRIPTION: Closes a file / directory / device
 *   INPUTS: fd to close
 *   OUTPUTS: none
 *   RETURN VALUE: 0 on success, -1 on error
 *   SIDE EFFECTS: Frees file descriptor for use
 */
int32_t close (int32_t fd)
{
    if(fd == 7)
        return COM1_close(fd);
    else if(fd == 8)
        return COM2_close(fd);
    else if(fd == 9)
        return sound_close(fd);      //sound
    else if (fd == 10)
        return mouse_close(fd);      // mouse
    else if (fd == 11)
    	return keyboard_close(fd);   // Keyboard
    else if (fd == 12)
    	return gui_close(fd); 	// Player engine

	/* 1. Find fd 2. call corresponding close */
	if (fd < 2 || fd > 7)
		return -1;

	//Get the current process
    file_array_t* file = get_cur_file(get_cur_process(), fd);

	if (file->flags != 0)
	{
		(*(file->f_ops.f_close))(fd);
		file->flags = 0;
	}
	else
		return -1;

	return 0;
}

/*
 * halt
 *   DESCRIPTION: Stops an executing program
 *   INPUTS: PID to halt
 *   OUTPUTS: none
 *   RETURN VALUE: Status of PID halted
 *   SIDE EFFECTS: none
 */
int32_t halt(uint8_t status)
{
    /* THE BIG HALT TO DO LIST
        close files
        decrement PID number to get to parent ID!
        reset page to parent
        flush TLB
        set tss.esp0/ss0
        load a 0 extended 8bits of eb to eax and leave/ret
        jump to last part of execute at end of halt
    */
    int32_t i;

    terminal_t* term = &(term_array[curr_term]);    // save address of whatever our current terminal is

    // CLOSE ALL OPEN FILES!
	pcb_t* cur_process = get_cur_process();

	// Clear Keyboard flags
	for (i = 0; i < NUM_NON_ASCII; i++)
		non_ascii[i] = 0;

    term->kb_redirect[0] = 0;
    term->kb_redirect[1] = 0;

    // Search for an filled entry in the file array
    for(i = 0; i < MAX_NUM_FILES; i++)
    {
        if(cur_process->f_descriptors[i].flags == 0x0001)   // LSB set means in use
        {
            cur_process->f_descriptors[i].flags = 0x0000; // set not in use
            file_array_t*file = get_cur_file(get_cur_process(), i);
            (*(file->f_ops.f_close))(i);    // call appropriate close function
        }
    }

    // RELEASE THE PROCESS FLAG
    term->process_flags[term->cur_pid] = 0; // set curpid flag to 0

    // REDUCE THE NUMBER OF RUNNNING PROCESSES
    num_processes--;

    // CHECK iF OUR PARENT IS OURSELF. what kind of screwed up stuff is going on here???
    if(term->cur_pid == cur_process->parent_pcb->pid)
    {
        asm volatile(
                "popl %%ebx;"
                "movzbl %%bl, %%eax;"
                "leave;"
                :
                :
                :"%eax","%ebx","memory"
                );
        uint8_t program[] = "shell";
        execute(program);
    }

    // DECREMENT PID to get to parent!
    term->cur_pid = cur_process->parent_pcb->pid; // change our cur_pid to equal the parent!

    // RESET PAGE TO PARENT - we need to offset PD to 32 to get a 128 MB virtual address offset
    page_directory[32] = (PROGRAM_START_ADDRESS + (curr_term * 4 * PROGRAM_ALLOC_SIZE) + (term->cur_pid) * PROGRAM_ALLOC_SIZE) | PRGM_PD_4MB_K_RW_P;

    // FLUSH TLB by reloading cr3
    asm volatile("                  \n\
			movl %0, %%eax          \n\
			movl %%eax, %%cr3       \n\
			"
			:
			:"r"(page_directory)
			:"%eax"
			);

    // set tss.esp0/ss0
    tss.esp0 = PROGRAM_START_ADDRESS - (curr_term * PROGRAM_KERN_STACK_SIZE * 4) - (PROGRAM_KERN_STACK_SIZE * term->cur_pid);

    //   load %bl->%eax, jump to last part of execute
	return 0;
}

/*
 * getargs
 *   DESCRIPTION: Fills buffer with passed in args
 *   INPUTS: Buffer to fill, max bytes to copy
 *   OUTPUTS: none
 *   RETURN VALUE: -1 fail, 0 success
 *   SIDE EFFECTS: none
 */
int32_t getargs (uint8_t* buf, int32_t nbytes)
{
	//Check if args are NULL
	if ( term_array[curr_term].args[0] == '\0')
		return -1;

	int i = 0;
	while(term_array[curr_term].args[i] != '\0')
	{
		// Cuz remember we need to put a NULL at the end
		if(i > (nbytes - 1))
			return -1;

		buf[i] = term_array[curr_term].args[i];
		i++;
	}

	buf[i] = '\0';
	return 0;
}

/*
 * vidmap
 *   DESCRIPTION: Maps video memory into a page
 *   INPUTS: Address to fill
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on erron, 0 on success
 *   SIDE EFFECTS: Sets up a page with video memory mapping
 */
int32_t vidmap (uint8_t** screen_start)
{
	/* Check address validity */
    if((screen_start == NULL) || (screen_start > (uint8_t**)VIRT_VIDEO_MEMORY) || (screen_start < (uint8_t**)VIRT_USERSPACE_START))
        return -1;

    /* Now create another page directory, pointing to text video mem (132-136MB in virtual memory)
	   In order to address this space, the page directory offset must be 33. One more than the
	   offset for the program space, 32 */

    page_directory[33] = ((uint32_t)second_page_table) | USER_LEVEL_PAGE_FLAGS;
	second_page_table[0] = ((unsigned int)term_array[active_term].vid_mem_ptr) | USER_LEVEL_PAGE_FLAGS;
	//second_page_table[0] = ((unsigned int)VID_MEM) | USER_LEVEL_PAGE_FLAGS;
	*screen_start = (uint8_t*)VIRT_VIDEO_MEMORY;

	return 0;
}

/*
 * set_handler
 *   DESCRIPTION: Sets up handler for device
 *   INPUTS: Signum to map, handler address to map to
 *   OUTPUTS: none
 *   RETURN VALUE: -1 (for now)
 *   SIDE EFFECTS: none
 */
int32_t set_handler (int32_t signum, void* handler_address)
{
  return -1;
}

/*
 * sigreturn
 *   DESCRIPTION: Returns from signal
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: -1 (for now)
 *   SIDE EFFECTS: none
 */
int32_t sigreturn (void)
{
  return -1;
}
