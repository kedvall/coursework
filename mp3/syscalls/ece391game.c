#include "ece391game.h"

#define color 1

uint8_t draw_start = 0;
uint8_t map_color_buf[MAP_WIDTH * MAP_HEIGHT];
uint8_t map_ascii_buf[MAP_WIDTH * MAP_HEIGHT];

int main()
{
	init_COM1();
	init_COM2();
	init_keyboard(); // Set up keyboard for redirection
	init_draw();
	init_players(); //Init players
	load_map(); // Load the map into our buffer

	while(1)
	{
		update_serial();
		update_player();
		draw_map();
		draw_player0();
		draw_player1();
	}

	return 0;
}


int32_t load_map()
{
	uint32_t i;
	uint8_t map[] = "basicMap2.txt";
	int32_t map_fd = ece391_open(map);  //Open map

	ece391_memset(map_color_buf, 0, sizeof(map_color_buf)); // Clear buffer
	ece391_memset(map_ascii_buf, 0, sizeof(map_ascii_buf)); // Fill with * buffer

	ece391_read(map_fd, map_color_buf, sizeof(map_color_buf)); //Read into buffer

	for(i = 0; i < sizeof(map_color_buf); i++) // replace with color
	{
		switch (map_color_buf[i])
		{
			case ' ':
				map_color_buf[i] = 0x33;
				break;
			case '.':
				map_color_buf[i] = 0x44;
				break;
			case '_':
				map_color_buf[i] = 0x11;
				break;
			case '|':
				map_color_buf[i] = 0x66; // brown
				break;
			case '+':
				map_color_buf[i] = 0x22; // green
				break;
			case '*':
				map_color_buf[i] = 0x55;
				break;
			case '@':
				map_color_buf[i] = 0xFF; // white
				break;
			default:
				map_color_buf[i] = 0x77;
				break;
		}
	}

	if(mp1_set_video_mode() == NULL)
        return -1;

	return 0;
}


void draw_map()
{
	ece391_vidcpy(vmem_base_addr, map_ascii_buf, draw_start);
	ece391_vidcpy(vmem_base_addr + 1, map_color_buf, draw_start);
}


void init_keyboard()
{
	kb_fd = ece391_open((uint8_t*)"KEYBOARD");
	keyboard_config = 1;
	ece391_write(kb_fd, &keyboard_config, 1);
}


void init_draw()
{
	draw_fd = ece391_open((uint8_t*)"PLAYER");
}

void init_COM1()
{
	ece391_open((uint8_t*)"COM1"); // open up our com1 port
}

void init_COM2()
{
	ece391_open((uint8_t*)"COM2"); // open up our com1 port
}


void update_serial()
{
    int j;
    int k = 1;

    int8_t read_buffer[256];
    uint8_t write_buffer[256];

    // this is just a giant delay
    for(j=0; j<4200000;j++)
        k = k*42;
    k = 1;

    write_buffer[0] = players[0].player_health;
    write_buffer[1] = players[0].player_x;
    write_buffer[2] = players[0].player_y;
    write_buffer[3] = players[0].player_state;

    players[1].player_health = ece391_read(7, read_buffer, 1);
    players[1].player_x = ece391_read(7, read_buffer, 1);
    players[1].player_y = ece391_read(7, read_buffer, 1);
    players[1].player_state = ece391_read(7, read_buffer, 1);

    // this writes something out to the buffer
    ece391_write(8, write_buffer, 4);
}
