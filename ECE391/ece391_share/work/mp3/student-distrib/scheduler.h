#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "types.h"
#include "global_includes.h"
#include "sys_calls.h"
#include "mouse.h"

#define BACKUP0 0xB9000
#define BACKUP1 0xBA000
#define BACKUP2 0xBB000

void scheduler_init();
void run_scheduler();

#endif
