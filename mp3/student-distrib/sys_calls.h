#ifndef _SYS_CALLS_H
#define _SYS_CALLS_H

#include "file.h"
#include "scheduler.h"
#include "paging.h"
#include "x86_desc.h"
#include "terminal.h"
#include "lib.h"
#include "rtc.h"
#include "process_interface.h"
#include "global_includes.h"
#include "serial.h"
#include "mouse.h"
#include "keyboard.h"
#include "sound.h"
#include "gui.h"

#define FILE_HEADER_SIZE 40
#define PROGRAM_START_ADDRESS 0x800000 // 8 MB
#define PROGRAM_KERN_STACK_SIZE 0x2000
#define PROGRAM_ALLOC_SIZE 0x400000
#define VIRT_PROGRAM_START 0x08048000
#define PRGM_PD_4MB_K_RW_P 0x87 //Attributes = Page size is 4MB, read / write, present
#define VIRT_VIDEO_MEMORY 0x8400000
#define VIRT_USERSPACE_START 0x8000000

extern int32_t halt(uint8_t status);
extern int32_t execute(const uint8_t* command);
extern int32_t read (int32_t fd, void* buf, int32_t nbytes);
extern int32_t write (int32_t fd, const void* buf, int32_t nbytes);
extern int32_t open (const int8_t* filename);
extern int32_t close (int32_t fd);
extern int32_t getargs (uint8_t* buf, int32_t nbytes);
extern int32_t vidmap (uint8_t** screenshot);
extern int32_t set_handler (int32_t signum, void* handler_address);
extern int32_t sigreturn (void);

extern void sys_call_execute(int instruction);
extern void halt_wrap(uint8_t status);

#endif
