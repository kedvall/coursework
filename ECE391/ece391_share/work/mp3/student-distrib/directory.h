#ifndef _DIRECTORY_H
#define _DIRECTORY_H

#include "types.h"
#include "global_includes.h"
//#include "scheduler.h"
#include "lib.h"
#include "terminal.h"

// GLOBAL variable used to save the start of the file system (THIS IS DEFINED IN KERNEL)
extern uint32_t* mod_start_address;

/* 64 bytes elapse between adjacent file names in the boot block */
#define BOOT_BLOCK_FILE_NAME_OFFSET 64

#define BLOCK_SIZE 4096 // <-- this is 4 KB right?
//#define FILE_SYSTEM_START_ADDRESS 0x40D000

extern int32_t directory_open(const uint8_t* filename);
extern int32_t directory_read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t directory_write(int32_t fd, const void* buf, int32_t nbytes);
extern int32_t directory_close(int32_t fd);

extern int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
extern int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
extern int32_t read_data(uint32_t* inode, uint32_t offset, uint8_t* buf, uint32_t length);

extern int32_t demo_filesys(uint8_t mode);
extern int32_t directory_valid();
extern uint32_t* get_inode_ptr(int32_t inode_num);

#endif
