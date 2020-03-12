#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#include "global_includes.h"
#include "scheduler.h"
#include "i8259.h"
#include "lib.h"
#include "rtc.h"
#include "directory.h"
#include "serial.h"

extern void keyboard_init();
extern void keyboardHandler();
extern void exitException();
void update_player(int32_t left, int32_t right);

int32_t keyboard_read(int32_t fd, void* buf, int32_t nbytes);
int32_t keyboard_open(const int8_t* filename);
int32_t keyboard_close(int32_t fd);
int32_t keyboard_write(int32_t fd, const void* buf, int32_t nbytes);

// extern uint8_t lineBuf[130];
// extern int linePtr;

#endif
