/*
MIT License

Copyright (c) 2026 Leafy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef LEAFY_RAYLIB_H
#define LEAFY_RAYLIB_H

#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// LOGGING
// ============================================================================
#define LF_LOG(...) TraceLog(LOG_INFO, __VA_ARGS__)
#define LF_WARN(...) TraceLog(LOG_WARNING, __VA_ARGS__)
#define LF_ERR(...) TraceLog(LOG_ERROR, __VA_ARGS__)

// ============================================================================
// SIMPLE TIMER
// ============================================================================
typedef struct {
  float time;
  float limit; // Time limit to reach
} LF_Timer;

static inline LF_Timer lf_timer(float limit) {
  return (LF_Timer){0.0f, limit};
};

static inline bool lf_timer_tick(LF_Timer *time, float dt) {
  time->time += dt;
  if (time->time >= time->limit) {
    time->time = 0.0f;
    return true;
  }

  return false;
}

static inline void lf_timer_reset(LF_Timer *timer) { timer->time = 0.0f; }
static inline float lf_timer_progress(LF_Timer *timer) {
  return (timer->limit > 0.0f) ? (timer->time / timer->limit) : 0.0f;
}

// ============================================================================
// VECTOR2 HELPERS
// ============================================================================
static inline Vector2 lf_vect(float x, float y) { return (Vector2){x, y}; };

// ============================================================================
// RECTANGLE HELPERS
// ============================================================================
static inline Rectangle lf_rect(float x, float y, float w, float h) {
  return (Rectangle){x, y, w, h};
};

static inline bool lf_rect_overlap(Rectangle a, Rectangle b) {
  return CheckCollisionRecs(a, b);
}

static inline bool lf_rect_contains(Rectangle r, Vector2 p) {
  return CheckCollisionPointRec(p, r);
}

// Get center of rectangle
static inline Vector2 lf_rect_center(Rectangle r) {
  return (Vector2){r.x + r.width * 0.5f, r.y + r.height * 0.5f};
}

static inline Rectangle lf_rect_expand(Rectangle rect, float amount) {
  rect.x -= amount;
  rect.y -= amount;
  rect.width += amount * 2;
  rect.height += amount * 2;
  return rect;
}

// ============================================================================
// INPUT
// ============================================================================

typedef struct {
  int left;
  int right;
  int up;
  int down;
  int jump;
  int action;
} LF_Controls;

static inline LF_Controls lf_default_control(void) {
  return (LF_Controls){
      .left = KEY_A,
      .right = KEY_D,
      .up = KEY_W,
      .down = KEY_S,
      .jump = KEY_SPACE,
      .action = KEY_Z,
  };
}

static inline bool lf_move_left(LF_Controls *c) {
  return IsKeyDown(c->left) || IsKeyDown(KEY_LEFT);
}
static inline bool lf_move_right(LF_Controls *c) {
  return IsKeyDown(c->right) || IsKeyDown(KEY_RIGHT);
}
static inline bool lf_move_up(LF_Controls *c) {
  return IsKeyDown(c->up) || IsKeyDown(KEY_UP);
}
static inline bool lf_move_down(LF_Controls *c) {
  return IsKeyDown(c->down) || IsKeyDown(KEY_DOWN);
}
static inline bool lf_jump(LF_Controls *c) { return IsKeyPressed(c->jump); }
static inline bool lf_action(LF_Controls *c) { return IsKeyPressed(c->action); }

// Get normalized movement vector (-1 to 1)
static inline Vector2 lf_move_vector(LF_Controls *c) {
  Vector2 v = {0.0f, 0.0f};
  if (lf_move_left(c))
    v.x -= 1.0f;
  if (lf_move_right(c))
    v.x += 1.0f;
  if (lf_move_up(c))
    v.y -= 1.0f;
  if (lf_move_down(c))
    v.y += 1.0f;
  return Vector2Normalize(v);
}

// ============================================================================
// CAMERA 2D HELPERS
// ============================================================================

static inline Camera2D lf_camera_follow(Vector2 target, float zoom) {
  Camera2D cam = {0};
  cam.target = target;
  cam.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  cam.rotation = 0.0f;
  cam.zoom = zoom;
  return cam;
}

// Smooth camera follow (lerp-based)
static inline void lf_camera_smooth_follow(Camera2D *cam, Vector2 target,
                                           float speed, float dt) {
  cam->target.x += (target.x - cam->target.x) * speed * dt;
  cam->target.y += (target.y - cam->target.y) * speed * dt;
}

static inline void lf_camera_begin(Camera2D cam) { BeginMode2D(cam); }
static inline void lf_camera_end(void) { EndMode2D(); }

// ============================================================================
// DEBUG DRAWING
// ============================================================================

// Draw a rectangle outline for debugging hitboxes
static inline void lf_draw_hitbox(Rectangle r, Color c) {
  DrawRectangleLinesEx(r, 2.0f, c);
}

// Draw a point/dot for debugging positions
static inline void lf_draw_point(Vector2 p, Color c) {
  DrawCircleV(p, 4.0f, c);
}

// Draw a grid for debugging world space
static inline void lf_draw_grid(int cell_size, Color c) {
  int w = GetScreenWidth();
  int h = GetScreenHeight();
  for (int x = 0; x < w; x += cell_size) {
    DrawLine(x, 0, x, h, c);
  }
  for (int y = 0; y < h; y += cell_size) {
    DrawLine(0, y, w, y, c);
  }
}

// ============================================================================
// RANDOM HELPERS
// ============================================================================

static inline int lf_rand_int(int min, int max) {
  return GetRandomValue(min, max);
}

static inline float lf_rand_float(float min, float max) {
  return min + (float)GetRandomValue(0, 10000) / 10000.0f * (max - min);
}

// probability p (0.0 to 1.0)
static inline bool lf_rand_chance(float p) {
  return lf_rand_float(0.0f, 1.0f) < p;
}

// ============================================================================
// ASSET LOADING SHORTCUTS
// ============================================================================

static inline Texture2D lf_load_texture(const char *path) {
  Texture2D texture = LoadTexture(path);
  if (texture.id == 0)
    LF_ERR("Failed to load texture: %s", path);
  else
    LF_LOG("Loaded texture: %s", path);

  return texture;
}

static inline Font lf_load_font(const char *path) {
  Font font = LoadFont(path);
  if (font.texture.id == 0)
    LF_ERR("Failed to load font: %s", path);
  else
    LF_LOG("Loaded font: %s", path);
  return font;
}

static inline Sound lf_load_sound(const char *path) {
  Sound sound = LoadSound(path);
  if (sound.frameCount == 0)
    LF_ERR("Failed to load sound: %s", path);
  else
    LF_LOG("Loaded sound: %s", path);
  return sound;
}

static inline Music lf_load_music(const char *path) {
  Music music = LoadMusicStream(path);
  if (music.frameCount == 0)
    LF_ERR("Failed to load music: %s", path);
  else
    LF_LOG("Loaded music: %s", path);
  return music;
}

// ============================================================================
// TILE/GRID HELPERS
// ============================================================================

typedef struct {
  int cols;      // Number of columns
  int rows;      // Number of rows
  int tile_size; // Size of each tile in pixels
} LF_Grid;

// Create a grid
static inline LF_Grid lf_grid(int cols, int rows, int tile_size) {
  return (LF_Grid){cols, rows, tile_size};
}

static inline Vector2 lf_grid_to_world(LF_Grid *grid, int col, int row) {
  return (Vector2){(float)(col * grid->tile_size),
                   (float)(row * grid->tile_size)};
}

// Convert world position to grid position
static inline Vector2 lf_world_to_grid(LF_Grid *grid, Vector2 pos) {
  return (Vector2){(float)((int)(pos.x / grid->tile_size)),
                   (float)((int)(pos.y / grid->tile_size))};
}

static inline Vector2 lf_grid_center(LF_Grid *grid, int col, int row) {
  float half = grid->tile_size * 0.5f;
  return (Vector2){(float)(col * grid->tile_size) + half,
                   (float)(row * grid->tile_size) + half};
}

static inline bool lf_grid_valid(LF_Grid *grid, int col, int row) {
  return col >= 0 && col < grid->cols && row >= 0 && row < grid->rows;
}

static inline Rectangle lf_grid_rect(LF_Grid *grid, int col, int row) {
  return (Rectangle){(float)(col * grid->tile_size),
                     (float)(row * grid->tile_size), (float)grid->tile_size,
                     (float)grid->tile_size};
}

static inline void lf_draw_tile(LF_Grid *grid, int col, int row, Color color) {
  Rectangle r = lf_grid_rect(grid, col, row);
  DrawRectangleRec(r, color);
}

static inline void lf_draw_tile_tex(LF_Grid *grid, int col, int row,
                                    Texture2D tex, Rectangle source) {
  Vector2 pos = lf_grid_to_world(grid, col, row);
  Rectangle dest = {pos.x, pos.y, (float)grid->tile_size,
                    (float)grid->tile_size};
  DrawTexturePro(tex, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

// Simple tilemap structure
typedef struct {
  LF_Grid grid;
  int *tiles; // 1D array of tile IDs (cols * rows)
} LF_Tilemap;

static inline LF_Tilemap lf_tilemap_create(int cols, int rows, int tile_size) {
  LF_Tilemap map = {0};
  map.grid = lf_grid(cols, rows, tile_size);
  map.tiles = (int *)MemAlloc(cols * rows * sizeof(int));
  for (int i = 0; i < cols * rows; i++) {
    map.tiles[i] = 0;
  }
  return map;
}

// Free tilemap memory
static inline void lf_tilemap_free(LF_Tilemap *map) {
  if (map->tiles) {
    MemFree(map->tiles);
    map->tiles = NULL;
  }
}

static inline int lf_tilemap_get(LF_Tilemap *map, int col, int row) {
  if (!lf_grid_valid(&map->grid, col, row))
    return -1;
  return map->tiles[row * map->grid.cols + col];
}

// Set tile at position
static inline void lf_tilemap_set(LF_Tilemap *map, int col, int row,
                                  int tile_id) {
  if (!lf_grid_valid(&map->grid, col, row))
    return;
  map->tiles[row * map->grid.cols + col] = tile_id;
}

static inline void lf_tilemap_draw(LF_Tilemap *map, Color colors[],
                                   int color_count) {
  for (int row = 0; row < map->grid.rows; row++) {
    for (int col = 0; col < map->grid.cols; col++) {
      int tile = lf_tilemap_get(map, col, row);
      if (tile > 0 && tile <= color_count) {
        lf_draw_tile(&map->grid, col, row, colors[tile - 1]);
      }
    }
  }
}

static inline void lf_tilemap_draw_tex(LF_Tilemap *map, Texture2D atlas,
                                       int atlas_cols, int atlas_tile_size) {
  for (int row = 0; row < map->grid.rows; row++) {
    for (int col = 0; col < map->grid.cols; col++) {
      int tile = lf_tilemap_get(map, col, row);
      if (tile > 0) {
        int src_col = (tile - 1) % atlas_cols;
        int src_row = (tile - 1) / atlas_cols;
        Rectangle source = {(float)(src_col * atlas_tile_size),
                            (float)(src_row * atlas_tile_size),
                            (float)atlas_tile_size, (float)atlas_tile_size};
        lf_draw_tile_tex(&map->grid, col, row, atlas, source);
      }
    }
  }
}

// ============================================================================
// ANIMATION HELPERS
// ============================================================================

typedef struct {
  Rectangle frame_rect;
  int frame_count;
  int current_frame;
  float frame_time;
  float timer;
  bool looping;
  bool finished;
} LF_Animation;

static inline LF_Animation lf_animation_create(int frames, int fps,
                                               float frame_width,
                                               float frame_height) {
  LF_Animation anim = {0};
  anim.frame_count = frames;
  anim.frame_time = 1.0f / fps;
  anim.frame_rect = (Rectangle){0, 0, frame_width, frame_height};
  anim.looping = true;
  return anim;
}

static inline void lf_animation_update(LF_Animation *anim, float dt) {
  if (anim->finished && !anim->looping)
    return;

  anim->timer += dt;
  if (anim->timer >= anim->frame_time) {
    anim->timer = 0;
    anim->current_frame++;

    if (anim->current_frame >= anim->frame_count) {
      if (anim->looping) {
        anim->current_frame = 0;
      } else {
        anim->current_frame = anim->frame_count - 1;
        anim->finished = true;
      }
    }
    anim->frame_rect.x = anim->current_frame * anim->frame_rect.width;
  }
}

static inline void lf_draw_animation(Texture2D tex, LF_Animation anim,
                                     Vector2 pos, float scale, Color tint) {
  Rectangle dest = {pos.x, pos.y, anim.frame_rect.width * scale,
                    anim.frame_rect.height * scale};
  Vector2 origin = {0, 0};
  DrawTexturePro(tex, anim.frame_rect, dest, origin, 0.0f, tint);
}
#endif
