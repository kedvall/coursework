#ifndef _GLOBAL_INCLUDES_H
#define _GLOBAL_INCLUDES_H

#include "types.h"

// THIS IS THE DEFINE YOU'RE LOOKING FOR IF YOU WANT TO DISABLE/ENABLE SCHEDULING
#define SCHEDULER_ON 0

#define NUM_OPERATIONS 4
#define MAX_NUM_PROCESSES 6
#define MAX_NUM_FILES 8
#define NUM_NON_ASCII 19

#define VID_MEM 0xB8000

#define DEFAULT_MOUSE_CHAR 0
#define DEFAULT_MOUSE_ATTR 0x1F

typedef struct file_struct
{
    int32_t (*f_open)(const uint8_t* filename);
    int32_t (*f_read)(int32_t fd, void* buf, int32_t nbytes);
    int32_t (*f_write)(int32_t fd, const void* buf, int32_t nbytes);
    int32_t (*f_close)(int32_t fd);
}op_pointers_t;

typedef struct file_array_object    // we have one of these for each file in use by the process
{
    op_pointers_t f_ops;
    uint32_t* inode;
    uint32_t f_pos;
    uint32_t flags;
} file_array_t; //LSB of flags is used to determine if it's in use

typedef struct process_ctrl_block   // each process has their own one of these that has info for it
{
    struct process_ctrl_block* parent_pcb;
    uint32_t pid;
    file_array_t f_descriptors[MAX_NUM_FILES];
    uint32_t ebp;
    uint32_t esp;
    // other stuff here as well (maybe)
} pcb_t;

typedef struct terminal
{
    uint8_t* vid_mem_ptr;
    //uint32_t cursor_pos;
    uint32_t screen_x;
    uint32_t screen_y;
    pcb_t* processes[MAX_NUM_PROCESSES];
    uint8_t process_flags[MAX_NUM_PROCESSES];
    int32_t cur_pid;
    uint8_t lineBuf[130];           // use active term struct only
    int linePtr;                // use active term struct only
    uint8_t args[128];  // this is in the sys_calls
    volatile int rtc_int_flag;
    uint8_t mouse_char;         // which character we display for the mouse pointer
    uint8_t mouse_attr;   // attributes we display for the mouse pointer
    uint8_t kb_redirect[1];     // read the other definition of it to find out more
} terminal_t;

typedef struct boot_block
{
  uint32_t num_entries;
  uint32_t num_inodes;
  uint32_t num_datablocks;
  uint8_t* first_dir_entry;
} boot_block_t;

typedef struct directory_entry
{
  uint8_t file_name[33];
  uint32_t file_type;
  uint32_t inode; //Inode number
} dentry_t;

extern terminal_t term_array[3];   // array of our terminals
extern uint32_t video_array[3]; //set this equal to vid mem backup addresses

extern uint8_t curr_term; // whichever term the scheduler is handling. not necessarily active
extern uint8_t active_term; // which terminal is active. all io (cursor, serial, keyboard) goes here. use this video memory

// uint8_t lineBuf[130];    //TERM_GLOBAL
// int linePtr;             //TERM_GLOBAL

// modifier keys are stored in an array as follows: [capslock, shift, ctrl, alt, enter]
extern uint8_t mod_keys[5];

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
extern uint8_t non_ascii[NUM_NON_ASCII];

extern int32_t mouse_x;         // holds mouse x coordinates
extern int32_t mouse_y;         // holds mouse y coordinates

extern void render_map();
extern int32_t player_pos;

#endif
