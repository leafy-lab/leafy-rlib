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

#endif
