#include "../include/leafy_raylib.h"
#include "raylib.h"

int main(void) {
  InitWindow(800, 600, "My Game");
  SetTargetFPS(60);

  LF_Controls controls = lf_default_control();

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    // Your game logic here

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Create a 20x15 grid with 32px tiles
    LF_Tilemap map = lf_tilemap_create(20, 15, 32);

    // Set some tiles (tile IDs start at 1, 0 is empty)
    lf_tilemap_set(&map, 5, 5, 1); // Wall
    lf_tilemap_set(&map, 6, 5, 2); // Floor

    // Draw with colors
    Color tile_colors[] = {BLACK, DARKGREEN, BROWN};
    lf_tilemap_draw(&map, tile_colors, 3);

    // Check if tile is solid
    int tile = lf_tilemap_get(&map, map.grid.cols, map.grid.rows);
    if (tile == 1) {
      // Collision with wall
    }

    // Cleanup
    lf_tilemap_free(&map);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
