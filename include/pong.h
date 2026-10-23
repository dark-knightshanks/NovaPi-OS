#ifndef PONG_H
#define PONG_H

#include <stdint.h>

// Represents the game ball
typedef struct {
  int x;  // Current x position
  int y;  // Current y position
  int dx; // Velocity in x direction
  int dy; // Velocity in y direction
  uint32_t radius;
  uint32_t color;
} Ball;

// Represents a player's paddle
typedef struct {
  int x;
  int y;
  int width;
  int height;
  uint32_t color;
} Player;

// Initializes the ball's starting state
void init_ball(Ball *ball, uint32_t radius, uint32_t color);

// Updates the ball's position and handles wall collisions
void update_ball_movement(Ball *ball);

// Initializes a player's paddle
void init_player(Player *player, int x, int y, int width, int height,
                 uint32_t color);

// Moves the player's paddle
void move_player(Player *player, int dy);

void start_pong_game();

#endif // PONG_H
