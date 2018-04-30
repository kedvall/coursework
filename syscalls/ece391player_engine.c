#include "ece391player_engine.h"

// Graphics
uint8_t attrib_array[16];

int32_t init_players()
{
	int32_t i;

	for (i = 0; i < 4; i++)
	{
		players[i].player_health = 100;
		players[i].player_x = i * 40;
		players[i].player_y = 19;
		players[i].player_state = 1;
	}

	ece391_memset(attrib_array, 0, 16);

	return 0;
}


void draw_player0()
{
		get_offset(0);
		get_attributes(0);
		int i;

		draw_buf0[0] = players[0].player_x;
		draw_buf0[1] = players[0].player_y;
		draw_buf0[2] = players[0].player_state;

		for (i = 0; i < 16; i++) // Copy 16 items
			draw_buf0[i + 3] = attrib_array[i]; //map_color_buf[get_offset()] & 0xF0;

		ece391_write(draw_fd, draw_buf0, 19);
}


void draw_player1()
{
		get_offset(1);
		get_attributes(1);
		int i;

		draw_buf1[0] = players[1].player_x;
		draw_buf1[1] = players[1].player_y;
		draw_buf1[2] = players[1].player_state;
		draw_buf1[19] = draw_start;

		for (i = 0; i < 16; i++) // Copy 16 items
			draw_buf1[i + 3] = attrib_array[i]; //map_color_buf[get_offset()] & 0xF0;

		ece391_write(draw_fd, draw_buf1, 20);
}


void get_attributes(uint8_t player)
{
	int i, j;

	uint32_t player_pos = players[player].player_x + (players[player].player_y * MAP_WIDTH);

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			attrib_array[j + (i * 4)] = map_color_buf[player_pos + (j + (i * MAP_WIDTH))];
		}
	}

	if (!player)
	{
		for (i = 0; i < 16; i++)
			attrib_array[i] &= 0xF0;
	} else {
		for (i = 0; i < 16; i++)
			attrib_array[i] &= 0xF5;
	}
}

int32_t get_offset(player)
{
	return (players[player].player_y * MAP_WIDTH) + players[player].player_x;
}


void update_player()
{
    uint8_t key;
	ece391_read(kb_fd, &key, 1);

	//Update position based on keypress
 	switch(key)
    {
    	case 200: // left
        	players[0].player_x-=2;
        	players[0].player_state = 0;
            break;
    	case 201: // Right
			players[0].player_x+=2;
        	players[0].player_state = 1;
	       break;
	    case 202: // UP
	        break;
	    case 203: // Down
	        break;
	    default:
	    	break;
	}

    /* Code dealing with upward movement */

	//Check for bounds overstepping
	if (players[0].player_x == 254)
		players[0].player_x = 0;
	else if (players[0].player_x > 152)
		players[0].player_x = 152;

	//Update where we start drawing on screen
	if (players[0].player_x >= 40 && players[0].player_x <= MAP_WIDTH - 41)
		draw_start = players[0].player_x - 40;
	else if (players[0].player_x < 40)
		draw_start = 0;
	else
		draw_start = MAP_WIDTH - 81;
}
