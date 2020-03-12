#include "ece391game_physics.h"

/* Y increases as it goes up, */
// +Y
// ^
// |
// |
// |
// |
// |
// ----------------> +X

// Static array to hold ground information
/* This array holds 2D info about where
 platforms/ground/wherever the player can stand at each x-coordinate.
 The 0th position holds the highest platform, while the 2nd holds the
 lowest ground.
 The physics functions will use this struct to more efficiently
 determine where and how to fall */
// static int ground[MAP_WIDTH][3];
//
// void init_ground_info()
// {
//     int32_t i, j;
//     for(i = 0; i < MAP_WIDTH; i++)
//     {
//         for(j = 0; j < 3; j++) // this line is for shayna
//         {
//             ground[i][j] = 0;
//         }
//     }
// }
//
// void create_ground_info(uint8_t* map)
// {
//     int32_t i, j;
//
//     // Outer loop iterates through width of map
//     for(i = 0; i < MAP_WIDTH; i++)
//     {
//         int iter = 0;
//         // Inner loop iterates through height of map
//         for(j = 0; j < MAP_HEIGHT; j++)
//         {
//             switch (map[i + (j * MAP_WIDTH)])
//             {
//                 case 0x44:
//                     ground[i][iter++] = j;
//                     break;
//                 case 0x22:
//                     ground[i][iter++] = j;
//                     break;
//                 case 0x66:
//                     ground[i][iter++] = j;
//                     break;
//                 case 0x11:
//                     ground[i][iter++] = j;
//                     break;
//                 default:
//                     break;
//             }
//             if(iter == 3)
//                 break;
//         }
//     }
// }
//
// // Given x, y coords - return what the ground position should be
// int acquire_ground(int x, int y)
// {
//     int32_t i;
//     for(i = 0; i < 3; i++)
//     {
//
//     }
// }

// player is a 4x4 block
int checkRight()
{
    // Check all 4 y-coordinates along the right edge of the player
    int i;
    for(i = 0; i < 4; i++)
    {
        switch (map_color_buf[players[0].player_x + 4 + ((players[0].player_y + i) * MAP_WIDTH)])
        {
            case 0x44:
                return 1;
                break;
            case 0x22:
                return 1;
                break;
            case 0x66:
                return 1;
                break;
            case 0x11:
                return 1;
                break;
            default:
                break;
        }
    }

    return 0;
}

int checkLeft()
{
    // Check all 4 y-coordinates along the left edge of the player
    int i;
    for(i = 0; i < 4; i++)
    {
        switch (map_color_buf[players[0].player_x - 1 + ((players[0].player_y + i) * MAP_WIDTH)])
        {
            case 0x44:
                return 1;
                break;
            case 0x22:
                return 1;
                break;
            case 0x66:
                return 1;
                break;
            case 0x11:
                return 1;
                break;
            default:
                break;
        }
    }

    return 0;
}

int checkUp()
{
    int i;
    for(i = 0; i < 4; i++)
    {
        switch (map_color_buf[players[0].player_x + i + ((players[0].player_y - 1) * MAP_WIDTH)]) {
            case 0x44:
                return 1;
                break;
            case 0x22:
                return 1;
                break;
            case 0x66:
                return 1;
                break;
            case 0x11:
                return 1;
                break;
            default:
                break;
        }
    }

    return 0;
}

int checkDown()
{
    int i;
    for(i = 0; i < 4; i++)
    {
        switch (map_color_buf[players[0].player_x + i + ((players[0].player_y + 4) * MAP_WIDTH)])
        {
            case 0x44:
                return 1;
                break;
            case 0x22:
                return 1;
                break;
            case 0x66:
                return 1;
                break;
            case 0x11:
                return 1;
                break;
            default:
                break;
        }
    }

    return 0;
}

// void jump()
// {
//
// }

static int timer = 10000;
// static int velocity_y = 0;
// static int hitGround = 1;
void update_player()
{
    uint8_t key;
	ece391_read(kb_fd, &key, 1);


	//Update position based on keypress
 	switch(key)
    {
    	case 200: // left
            // timer--;
            // if(!timer)
            // {
            //     players[0].player_state = (players[0].player_state == LEFT_1) ? LEFT_2 : LEFT_1;
            //     timer = 10000;
            // }
            //if(!checkLeft()) // check left
                  players[0].player_x -= 2;
            break;
    	case 201: // Right
            // timer--;
            // if(!timer)
            // {
            //     players[0].player_state = (players[0].player_state == RIGHT_1) ? RIGHT_2 : RIGHT_1;
            //     timer = 10000;
            // }
            //if(!checkRight()) // check right
    		      players[0].player_x += 2;
	       break;
	    case 202: // UP
            // if(hitGround)
            // {
            //     hitGround = 0;
            //     jump();
            // }
	        break;
	    case 203: // Down
	        break;
	    default:
	    	break;
	}

    /* Code dealing with upward movement */

	//Check for bounds overstepping
	if (players[0].player_x < 1)
		players[0].player_x = 0;
	else if (players[0].player_x > 226)
		players[0].player_x = 226;

	//Update where we start drawing on screen
	if (players[0].player_x >= 40 && players[0].player_x <= MAP_WIDTH - 41)
		draw_start = players[0].player_x - 40;
	else if (players[0].player_x < 40)
		draw_start = 0;
	else
		draw_start = MAP_WIDTH - 81;
}
