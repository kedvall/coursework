#include "gui.h"

// Player graphics
static uint8_t player_left_s[16] = {'3','-','-','-',' ','O','/',' ','/','|',' ',' ','/',' ','\\',' '};
//static uint8_t player_left_1[16] = {'3','-','-','-',' ','O','/',' ','/','|',' ',' ','/','<',' ',' '};
//static uint8_t player_left_2[16] = {'3','-','-','-',' ','O','/',' ','/','|',' ',' ','<','\\',' ',' '};

static uint8_t player_right_s[16] = {'-','-','-','E',' ','O','/',' ','/','|',' ',' ','/',' ','\\',' '};
//static uint8_t player_right_1[16] = {'-','-','-','E',' ','O','/',' ','/','|',' ',' ',' ','/','>',' '};
//static uint8_t player_right_2[16] = {'-','-','-','E',' ','O','/',' ','/','|',' ',' ',' ','>','\\',' '};

//static uint8_t pitchfork_left[4] = {'3','-','-','-'};
//static uint8_t pitchfork_right[4] = {'-','-','-','E'};

static int8_t draw_start = 0;

int32_t gui_open(const int8_t* filename)
{
    player = 0;
    return 12;
}

int32_t gui_close(int32_t fd)
{
    return 0;
}

int32_t gui_read(int32_t fd, void* buf, int32_t nbytes)
{
    return 0;
}

int32_t gui_write(int32_t fd, const void* buf, int32_t nbytes)
{
    uint8_t* cbuf = (uint8_t*)buf;
    int i;

    if (!player)
    {
        for (i = 0; i < 19; i++)
            player_data0[i] = cbuf[i];
    } else {
        for (i = 0; i < 19; i++)
            player_data1[i] = cbuf[i];
    }

    player = !player;
    draw_start = cbuf[19];
    return 0;
}

void player_display()
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (player_data0[2])
            {
                if(draw_start < player_data0[0])
                {
                    *(uint8_t *)(VID_MEM + (160 * player_data0[1] + player_data0[0]) + ((j << 1)) + (i * 160)) = player_right_s[j + (i * 4)];
                    *(uint8_t *)(VID_MEM + (160 * player_data0[1] + player_data0[0]) + ((j << 1) + 1) + (i * 160)) = player_data0[j + (i * 4) + 3];
                }
            } else
            {
                if(draw_start < player_data0[0])
                {
                    *(uint8_t *)(VID_MEM + (160 * player_data0[1] + player_data0[0]) + ((j << 1)) + (i * 160)) = player_left_s[j + (i * 4)];
                    *(uint8_t *)(VID_MEM + (160 * player_data0[1] + player_data0[0]) + ((j << 1) + 1) + (i * 160)) = player_data0[j + (i * 4) + 3];
                }
            }
        }
    }

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (player_data1[2])
            {
                if(draw_start < player_data1[0])
                {
                    *(uint8_t *)(VID_MEM + (160 * player_data1[1] + player_data1[0]) + ((j << 1)) + (i * 160)) = player_right_s[j + (i * 4)];
                    *(uint8_t *)(VID_MEM + (160 * player_data1[1] + player_data1[0]) + ((j << 1) + 1) + (i * 160)) = player_data1[j + (i * 4) + 3];
                }
            } else {
                if(draw_start < player_data1[0])
                {
                    *(uint8_t *)(VID_MEM + (160 * player_data1[1] + player_data1[0]) + ((j << 1)) + (i * 160)) = player_left_s[j + (i * 4)];
                    *(uint8_t *)(VID_MEM + (160 * player_data1[1] + player_data1[0]) + ((j << 1) + 1) + (i * 160)) = player_data1[j + (i * 4) + 3];
                }
            }
        }
    }

}
