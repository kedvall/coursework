#ifndef _TERM_DRIVER_H
#define _TERM_DRIVER_H

#include "types.h"
#include "global_includes.h"
#include "lib.h"

extern int32_t term_open(const uint8_t* filename);
extern int32_t term_read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t term_write(int32_t fd, const void* buf, int32_t nbytes);
extern int32_t term_close(int32_t fd);

// extern uint8_t lineBuf[130];
// extern int linePtr;

#endif
