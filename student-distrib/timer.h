#include "lib.h"
#include "types.h"
#include "global_includes.h"
#include "scheduler.h"
#include "i8259.h"
#include "gui.h"

#define TIMER_FREQ 100
#define TIME_SLICE 5	//50 milliseconds

extern void init_PIT();
extern void timer_handler();
extern void sleep_deciseconds(uint32_t deciseconds);
