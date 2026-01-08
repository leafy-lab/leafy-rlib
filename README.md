# 🍃 leafy-raylib

A **lightweight, header-only helper library** for [raylib](https://www.raylib.com/) in C.  
Designed to reduce boilerplate and make common game development tasks easier for beginners.

## ✨ Features

- 🎮 **Input Helpers** - Simplified keyboard controls with customizable keybindings
- 📦 **Grid/Tilemap System** - Easy top-down tile-based games with no bloat
- ⏱️ **Simple Timers** - Clean countdown timers for cooldowns and events
- 🎥 **Camera Utilities** - Smooth camera follow and setup helpers
- 🔧 **Math Shortcuts** - Vector2, Rectangle helpers, and collision detection
- 🎨 **Debug Drawing** - Hitbox visualization, grids, and debug points
- 🎲 **Random Utilities** - Int, float, and probability helpers
- 📁 **Asset Loading** - Simplified loading with automatic error logging

All functions are **`static inline`** to avoid linkage issues and are prefixed with `lf_` to prevent naming conflicts.

## 📦 Installation

### Option 1: Copy the header
Download `leafy_raylib.h` and include it in your project:
```c
#include "leafy_raylib.h"
```

### Option 2: Install system-wide (Linux/macOS)
```bash
git clone https://github.com/yourname/leafy-raylib.git
cd leafy-raylib
chmod +x install.sh
./install.sh
```

This copies the header to `/usr/local/include/` for system-wide access.

## 📚 Usage

### Basic Setup
```c
#include "raylib.h"
#include "leafy_raylib.h"

int main(void) {
    InitWindow(800, 600, "My Game");
    SetTargetFPS(60);
    
    LF_Controls controls = lf_default_control();
    
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        
        // Your game logic here
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Your drawing here
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
```

### Input Handling
```c
LF_Controls controls = lf_default_control(); // WASD + Arrow keys + Space + Z

if (lf_move_left(&controls)) {
    player.x -= speed * dt;
}

Vector2 move = lf_move_vector(&controls); // Get normalized direction
player.x += move.x * speed * dt;
player.y += move.y * speed * dt;

if (lf_jump(&controls)) {
    // Jump logic
}
```

### Timers
```c
LF_Timer shoot_cooldown = lf_timer(0.5f); // 0.5 second cooldown

// In update loop:
if (lf_timer_tick(&shoot_cooldown, dt)) {
    // Fire bullet
}

// Check progress (0.0 to 1.0)
float progress = lf_timer_progress(&shoot_cooldown);
```

### Grid/Tilemap System
```c
// Create a 20x15 grid with 32px tiles
LF_Tilemap map = lf_tilemap_create(20, 15, 32);

// Set some tiles (tile IDs start at 1, 0 is empty)
lf_tilemap_set(&map, 5, 5, 1); // Wall
lf_tilemap_set(&map, 6, 5, 2); // Floor

// Draw with colors
Color tile_colors[] = {GRAY, DARKGREEN, BROWN};
lf_tilemap_draw(&map, tile_colors, 3);

// Or draw with texture atlas
Texture2D tiles = LoadTexture("tileset.png");
lf_tilemap_draw_tex(&map, tiles, 8, 32); // 8 cols, 32px tile size

// Convert positions
Vector2 world_pos = lf_grid_to_world(&map.grid, 5, 5);
Vector2 grid_pos = lf_world_to_grid(&map.grid, player_pos);

// Check if tile is solid
int tile = lf_tilemap_get(&map, grid_x, grid_y);
if (tile == 1) {
    // Collision with wall
}

// Cleanup
lf_tilemap_free(&map);
```

### Camera
```c
Camera2D camera = lf_camera_follow(player_pos, 2.0f);

// Smooth follow
lf_camera_smooth_follow(&camera, player_pos, 5.0f, dt);

// Use camera
lf_camera_begin(camera);
// Draw game world
lf_camera_end();
```

### Collision & Math
```c
Rectangle player = lf_rect(x, y, 32, 32);
Rectangle enemy = lf_rect(ex, ey, 32, 32);

if (lf_rect_overlap(player, enemy)) {
    // Collision detected
}

if (lf_rect_contains(button, GetMousePosition())) {
    // Mouse over button
}

Vector2 center = lf_rect_center(player);
```

### Random Utilities
```c
int dice = lf_rand_int(1, 6);
float speed = lf_rand_float(100.0f, 200.0f);

if (lf_rand_chance(0.3f)) { // 30% chance
    // Spawn enemy
}
```

### Debug Drawing
```c
lf_draw_hitbox(player_rect, RED);
lf_draw_point(spawn_pos, GREEN);
lf_draw_grid(32, LIGHTGRAY); // 32px grid
lf_draw_tile_grid(&map.grid, ColorAlpha(BLUE, 0.5f));
```

## 🎮 Example: Simple Top-Down Game
```c
#include "raylib.h"
#include "leafy_raylib.h"

int main(void) {
    InitWindow(800, 600, "Top-Down Game");
    SetTargetFPS(60);
    
    LF_Controls controls = lf_default_control();
    LF_Tilemap map = lf_tilemap_create(25, 18, 32);
    
    // Create walls around the edge
    for (int i = 0; i < map.grid.cols; i++) {
        lf_tilemap_set(&map, i, 0, 1);
        lf_tilemap_set(&map, i, map.grid.rows - 1, 1);
    }
    for (int i = 0; i < map.grid.rows; i++) {
        lf_tilemap_set(&map, 0, i, 1);
        lf_tilemap_set(&map, map.grid.cols - 1, i, 1);
    }
    
    Vector2 player = lf_grid_center(&map.grid, 12, 9);
    Camera2D camera = lf_camera_follow(player, 2.0f);
    
    Color tiles[] = {GRAY, DARKGREEN};
    
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        
        // Movement
        Vector2 move = lf_move_vector(&controls);
        Vector2 new_pos = Vector2Add(player, Vector2Scale(move, 200.0f * dt));
        
        // Check collision
        Vector2 grid_pos = lf_world_to_grid(&map.grid, new_pos);
        if (lf_tilemap_get(&map, grid_pos.x, grid_pos.y) == 0) {
            player = new_pos;
        }
        
        lf_camera_smooth_follow(&camera, player, 5.0f, dt);
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        lf_camera_begin(camera);
        lf_tilemap_draw(&map, tiles, 2);
        DrawCircleV(player, 12, YELLOW);
        lf_camera_end();
        
        DrawText("WASD to move", 10, 10, 20, WHITE);
        
        EndDrawing();
    }
    
    lf_tilemap_free(&map);
    CloseWindow();
    return 0;
}
```

## 🛠️ Requirements

- [raylib](https://www.raylib.com/) (tested with raylib 5.0+)
- C compiler (gcc, clang, msvc)

## 📜 License

MIT License

## 🤝 Contributing

Contributions welcome! Keep the library simple, beginner-friendly, and header-only.

## 🔗 Links

- [raylib official website](https://www.raylib.com/)
- [raylib documentation](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [raylib examples](https://www.raylib.com/examples.html)

---

Made with 🍃 for raylib beginners
```
