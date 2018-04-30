#ifndef GUI_H
#define GUI_H

#include "types.h"
#include "lib.h"
#include "global_includes.h"

uint8_t player;
uint8_t player_data0[19];
uint8_t player_data1[19];

int32_t gui_open(const int8_t* filename);
int32_t gui_close(int32_t fd);
int32_t gui_read(int32_t fd, void* buf, int32_t nbytes);
int32_t gui_write(int32_t fd, const void* buf, int32_t nbytes);
void player_display();

#endif
