#include "../include/leafy-raylib.h"
#include <raylib.h>

int main(void) {
  InitWindow(800, 450, "Leafy Controls Example");
  SetTargetFPS(60);

  // Player rectangle
  Rectangle player = lf_rect(375, 200, 50, 50);
  float speed = 200.0f;

  // Optional: velocity vector
  Vector2 velocity = lf_vect(0, 0);

  // Timer to change debug color every second
  LF_Timer color_timer = lf_timer(1.0f);
  bool is_green = true;
  Color player_color = GREEN;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    // -------------------------
    // Controls (WASD)
    // -------------------------
    velocity = lf_vect(0, 0); // reset each frame

    if (IsKeyDown(KEY_A))
      velocity.x -= speed;
    if (IsKeyDown(KEY_D))
      velocity.x += speed;
    if (IsKeyDown(KEY_W))
      velocity.y -= speed;
    if (IsKeyDown(KEY_S))
      velocity.y += speed;

    // Apply movement
    player.x += velocity.x * dt;
    player.y += velocity.y * dt;

    // Keep player inside screen bounds
    if (player.x < 0)
      player.x = 0;
    if (player.y < 0)
      player.y = 0;
    if (player.x + player.width > GetScreenWidth())
      player.x = GetScreenWidth() - player.width;
    if (player.y + player.height > GetScreenHeight())
      player.y = GetScreenHeight() - player.height;

    // -------------------------
    // Timer to toggle color
    // -------------------------
    if (lf_timer_tick(&color_timer, dt)) {
      is_green = !is_green;
      player_color = is_green ? GREEN : RED;
      LF_LOG("Player color toggled");
    }

    // -------------------------
    // Drawing
    // -------------------------
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw player rectangle
    DrawRectangleRec(player, player_color);

    // Debug helpers
    lf_draw_hitbox(player, SKYBLUE);               // outline
    lf_draw_point(lf_rect_center(player), YELLOW); // center dot
    lf_draw_grid(50, DARKGRAY);                    // grid

    // Show velocity vector
    DrawLineV(lf_rect_center(player),
              lf_vect(lf_rect_center(player).x + velocity.x * 0.1f,
                      lf_rect_center(player).y + velocity.y * 0.1f),
              RED);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
