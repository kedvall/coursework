#if !defined(_PLAYER_ENGINE_H)
#define _PLAYER_ENGINE_H

#include <stdint.h>
#include "ece391support.h"
#include "ece391syscall.h"

#define PLAYER_W 4
#define PLAYER_H 4

typedef enum int8_t {LEFT_1, LEFT_2, LEFT_S, RIGHT_1, RIGHT_2, RIGHT_S} player_state_t;

typedef struct player_struct
{
	uint8_t player_health;
	uint8_t player_x;
	uint8_t player_y;
	uint8_t player_state;
} player_t;

// Array to hold players
player_t players[4];

uint8_t draw_start;
uint8_t map_color_buf[MAP_WIDTH * MAP_HEIGHT];
uint8_t map_ascii_buf[MAP_WIDTH * MAP_HEIGHT];
uint8_t attrib_array[16];

int32_t draw_fd;
uint8_t draw_buf0[20];
uint8_t draw_buf1[20];

// Keyboard vars
int32_t kb_fd;
uint8_t keyboard_config;
uint8_t key;

int32_t ece391_playercpy(void* dest, const void* src, uint32_t start);
void draw_players();
int32_t init_players();
void get_attributes(uint8_t player);
int32_t get_offset();
void draw_player0();
void draw_player1();

#endif
