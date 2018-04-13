#include "timer.h"

static uint32_t timer_ticks;

void init_PIT()
{
    enable_irq(0);
    uint32_t freq;
    timer_ticks = 0;
    freq = 11932; // frequency is 100 Hz

    outb(0x36, 0x43);
    outb(freq & 0xFF, 0x40);
    outb(freq >> 8, 0x40);
}

void timer_handler()
{
    timer_ticks++;
    // if(timer_ticks % TIMER_FREQ == 0)
    //     puts("One second has passed!");
    send_eoi(0);

    if(!SCHEDULER_ON)      // if scheduler is not on, do some video copying and mouse pointing still
    {
        if (timer_ticks % 5)
        {
            memcpy((void*)VID_MEM, (void*)video_array[0], NUM_COLS * NUM_ROWS * 2);
            player_display();
            mouse_display();
        }
        return;
    }

    if (!(timer_ticks % 2)) // every 20 milliseconds AKA 2 ticks
    {
        run_scheduler();
    }

    return;
}

void sleep_deciseconds(uint32_t deciseconds)
{
    uint32_t current_tick = timer_ticks;
    while((timer_ticks - current_tick) < deciseconds);
}
