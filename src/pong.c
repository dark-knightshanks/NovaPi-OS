#include "pong.h"
#include "mailbox.h"
#include "peripherals/mailbox.h" // For screen dimensions
#include "utils.h"
#include "video.h"

// Extern the framebuffer info to get screen dimensions
extern volatile mailbox_fb_request fb1;

#define BACKGROUND_COLOR 0x00000000 // Assuming a black background
#define BORDER_MARGIN 40

void init_ball(Ball *ball, uint32_t radius, uint32_t color) {
  // Start the ball in the center of the screen
  ball->x = fb1.res.width / 2;
  ball->y = fb1.res.height / 2;

  // Initial velocity - adjust these values for desired speed
  ball->dx = 2;
  ball->dy = 2;

  ball->radius = radius;
  ball->color = color;
}

void update_ball_movement(Ball *ball) {
  // 1. Clear the ball from its old position using the background color
  draw_circle((uint32_t)ball->x, (uint32_t)ball->y, ball->radius,
              BACKGROUND_COLOR);

  // 2. Update position based on velocity
  ball->x += ball->dx;
  ball->y += ball->dy;

  // 3. Check for collisions with screen edges and reverse direction
  // The border width of 40 is assumed from your 'screen_border' function.
  uint32_t border_margin = 40;

  // Collision with top or bottom wall
  if ((ball->y - ball->radius) <= border_margin ||
      (ball->y + ball->radius) >= (fb1.res.height - border_margin)) {
    ball->dy = -ball->dy;
  }

  // Collision with left or right wall
  // In a full Pong game, hitting these walls would typically mean a point for
  // the opponent
  if ((ball->x - ball->radius) <= border_margin ||
      (ball->x + ball->radius) >= (fb1.res.width - border_margin)) {
    ball->dx = -ball->dx;
  }

  // 4. Draw the ball in its new position with its specified color
  draw_circle((uint32_t)ball->x, (uint32_t)ball->y, ball->radius, ball->color);
}

void init_player(Player *player, int x, int y, int width, int height,
                 uint32_t color) {
  player->x = x;
  player->y = y;
  player->width = width;
  player->height = height;
  player->color = color;
  draw_player_paddle(player->x, player->y, player->height, player->width,
                     player->color);
}

void move_player(Player *player, int dy) {
  // Erase the paddle from its old position
  draw_player_paddle(player->x, player->y, player->height, player->width,
                     BACKGROUND_COLOR);

  // Update the player's y position
  player->y += dy;

  // Boundary checking to keep the paddle on the screen
  if (player->y < BORDER_MARGIN) {
    player->y = BORDER_MARGIN;
  }
  if (player->y + player->height > fb1.res.height - BORDER_MARGIN) {
    player->y = fb1.res.height - BORDER_MARGIN - player->height;
  }

  // Draw the paddle in its new position
  draw_player_paddle(player->x, player->y, player->height, player->width,
                     player->color);
}

Ball game_ball;
Player player1;
Player player2;

void start_pong_game(void) {
  mail_framebuffer(1280, 720, 32);
  draw_game_board(1280, 720);
  init_ball(&game_ball, 10, 0x457B9DFF); // 10 is radius, 0x457B9DFF ball color
  init_player(&player1, 50, 250, 10, 30, 0xFFFFFFFF);   // Left paddle
  init_player(&player2, 1220, 250, 10, 30, 0xFFFFFFFF); // Right paddle

  int p1_dy = 2;
  int p2_dy = -2;

  while (1) {
    update_ball_movement(&game_ball);

    // Move players
    move_player(&player1, p1_dy);
    move_player(&player2, p2_dy);

    // Simple AI to make paddles bounce
    if (player1.y <= BORDER_MARGIN ||
        player1.y + player1.height >= 60 - BORDER_MARGIN) {
      p1_dy = -p1_dy;
    }
    if (player2.y <= BORDER_MARGIN ||
        player2.y + player2.height >= 60 - BORDER_MARGIN) {
      p2_dy = -p2_dy;
    }

    delay(1000000);
  }
}
