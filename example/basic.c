#include "../include/leafy-raylib.h"
#include <raylib.h>

int main(void) {
  InitWindow(800, 450, "leafy-raylib basic");
  SetTargetFPS(60);

  Rectangle player = lf_rect(100, 200, 50, 50);
  Vector2 velocity = lf_vect(200, 0);

  LF_Timer color_timer = lf_timer(1.0f);
  Color player_color = GREEN;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    // Movement
    player.x += velocity.x * dt;

    // Bounce on screen edges
    if (player.x < 0 || player.x + player.width > GetScreenWidth()) {
      velocity.x = -velocity.x;
    }

    // Timer example (change color every second)
    if (lf_timer_tick(&color_timer, dt)) {
      if (player_color.r == GREEN.r && player_color.g == GREEN.g &&
          player_color.b == GREEN.b) {
        player_color = RED;
      } else {
        player_color = GREEN;
      }
      LF_LOG("Color toggled");
    }

    BeginDrawing();
    ClearBackground(BLACK);
    lf_draw_grid(64, WHITE);
    DrawRectangleRec(player, player_color);

    // Debug helpers
    Vector2 center = lf_rect_center(player);
    DrawCircleV(center, 3, YELLOW);

    Rectangle expanded = lf_rect_expand(player, 5);
    DrawRectangleLinesEx(expanded, 1.0f, SKYBLUE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
