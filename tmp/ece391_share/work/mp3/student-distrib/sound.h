#ifndef _SOUND_H
#define _SOUND_H

#include "types.h"
#include "global_includes.h"
#include "scheduler.h"
#include "lib.h"
#include "rtc.h"
#include "timer.h"

//extern void beep();
// extern void betterBeep(int freq);
// extern void silence();

extern int32_t sound_open (const int8_t* filename);
extern int32_t sound_write (int32_t fd, const void* buf, int32_t nbytes);
extern int32_t sound_read (int32_t fd, void* buf, int32_t nbytes);
extern int32_t sound_close (int32_t fd);
#endif
