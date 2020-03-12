#include <stdio.h>
#include <stdint.h>

#define SCREEN_W 80
#define SCREEN_H 25
#define MAP_W 16
#define MAP_H 25

uint8_t attrib_array[16];
uint8_t map_color_buf[16] = {0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44};

int main()
{
    int32_t i, j;
	uint32_t player_pos = 0;

	for (i = 0; i < 16; i++)
		printf("init buf: %X\n", map_color_buf[i]);

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			attrib_array[j + (i * 4)] = map_color_buf[player_pos + (j + (i * MAP_W))];
	}

	for (i = 0; i < 16; i++)
		printf("Attrib: %X\n", attrib_array[i]);

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			attrib_array[j + (i * 4)] = attrib_array[j + (i * 4)] & 0xF7;
	}

	for (i = 0; i < 16; i++)
		printf("Buf: %X\n", attrib_array[i]);
}

