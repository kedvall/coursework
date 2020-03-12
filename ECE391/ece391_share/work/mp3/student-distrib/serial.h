#ifndef _SERIAL_H
#define _SERIAL_H

#include "types.h"
#include "global_includes.h"
#include "scheduler.h"
#include "i8259.h"
#include "lib.h"
#include "rtc.h"
#include "process_interface.h"

extern void init_serial(int com, uint32_t baud_rate);

// local functions
int serial_inbox(int com);
int serial_outbox(int com);
char serial_receive(int com);
extern void serial_send(int com, char message);

int32_t COM1_open (const uint8_t* filename);
int32_t COM1_read (int32_t fd, void* buf, int32_t nbytes);
int32_t COM1_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t COM1_close (int32_t fd);

int32_t COM2_open (const uint8_t* filename);
int32_t COM2_read (int32_t fd, void* buf, int32_t nbytes);
int32_t COM2_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t COM2_close (int32_t fd);

#endif
