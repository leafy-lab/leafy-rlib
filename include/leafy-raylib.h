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
  return (LF_Timer{0.0f, limit});
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

#endif
