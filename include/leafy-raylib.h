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

#endif
