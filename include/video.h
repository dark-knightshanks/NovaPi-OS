#ifndef _VIDEO_H
#define _VIDEO_H

#include "peripherals/mailbox.h"
#include <stdbool.h>
#include <stdint.h>

extern void draw_pixels(uint32_t x, uint16_t y, uint32_t color);
extern void video_draw_char(char c, uint32_t pos_x, uint32_t pos_y);
extern void video_draw_string(char *s, uint32_t pos_x, uint32_t pos_y);
extern void video_render(uint32_t xres, uint32_t yres);
extern void draw_circle(uint32_t x_pos, uint32_t y_pos, uint32_t rad,
                        uint32_t color);
extern void ball_random_movement(uint32_t rad, uint32_t color);
extern void draw_rectangle(uint32_t x_pos, uint32_t y_pos, uint32_t h,
                           uint32_t w, uint32_t color);
extern void block_movement(int h, int w);
extern void screen_border(uint32_t xres, uint32_t yres);
extern void draw_game_board(uint32_t xres, uint32_t yres);
extern void draw_player_paddle(int x, int y, int h, int w, int color);

#endif
