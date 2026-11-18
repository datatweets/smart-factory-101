// vib_logic.h - Lab 5: bearing vibration watch. Pure maths, no Arduino calls.
// Two ways to catch a failing bearing:
//   1) a LIMIT   - "above 280 mg is an alert"
//   2) a TREND   - "three weekly readings in a row, each higher, and clearly up"
// The trend rule warns earlier. That is the idea behind condition-based maintenance.
#pragma once
#include <math.h>

struct Vec { float x, y, z; };          // acceleration in g

// Limits sit between the simulator slider steps (50 mg) so readings never land exactly on a limit.
const float WATCH_MG = 130.0f;           // milli-g away from the healthy baseline
const float ALERT_MG = 280.0f;
const float HYST_MG  = 25.0f;
const float TREND_MIN_RISE_MG = 40.0f;   // ignore tiny wobbles

enum Level { LV_NORMAL = 0, LV_WATCH = 1, LV_ALERT = 2 };

// How far the machine is from its healthy "baseline" pose, in milli-g.
inline float deviationMg(Vec a, Vec base) {
  float dx = a.x - base.x, dy = a.y - base.y, dz = a.z - base.z;
  return sqrtf(dx * dx + dy * dy + dz * dz) * 1000.0f;
}

inline Level updateLevel(Level lv, float mg) {
  for (int i = 0; i < 2; i++) {
    if (lv == LV_NORMAL && mg >= WATCH_MG) lv = LV_WATCH;
    else if (lv == LV_WATCH && mg >= ALERT_MG) lv = LV_ALERT;
    else if (lv == LV_ALERT && mg < ALERT_MG - HYST_MG) lv = LV_WATCH;
    else if (lv == LV_WATCH && mg < WATCH_MG - HYST_MG) lv = LV_NORMAL;
  }
  return lv;
}

const int MAX_WEEKS = 10;
struct Trend { float v[MAX_WEEKS]; int n; };

inline bool addWeek(Trend &t, float mg) {
  if (t.n >= MAX_WEEKS) return false;
  t.v[t.n++] = mg;
  return true;
}

// Rising = last three readings strictly increasing AND at least TREND_MIN_RISE_MG higher than three weeks ago.
inline bool rising(const Trend &t) {
  if (t.n < 3) return false;
  float a = t.v[t.n - 3], b = t.v[t.n - 2], c = t.v[t.n - 1];
  return (b > a) && (c > b) && ((c - a) >= TREND_MIN_RISE_MG);
}
