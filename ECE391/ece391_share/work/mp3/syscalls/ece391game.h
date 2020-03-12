#ifndef _GAME_H
#define _GAME_H

#include "ece391player_engine.h"
#include "ece391game_physics.h"

int32_t load_map();
void draw_map();
void update_player();
void init_keyboard();
void init_draw();
void init_COM1();
void init_COM2();
void update_serial();

#endif
