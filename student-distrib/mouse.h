#ifndef _MOUSE_H
#define _MOUSE_H

#include "types.h"
#include "global_includes.h"
#include "i8259.h"
#include "lib.h"
#include "rtc.h"
#include "process_interface.h"
#include "lib.h"
#include "sys_calls.h"

extern void mouse_init();
void mouse_display();

// local functions
inline void mouse_wait(uint8_t a_type);
inline void write_mouse(uint8_t a_write);
uint8_t read_mouse();

int32_t mouse_open (const uint8_t* filename);
int32_t mouse_read (int32_t fd, void* buf, int32_t nbytes);
int32_t mouse_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t mouse_close (int32_t fd);

#endif
