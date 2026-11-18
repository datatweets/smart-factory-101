// roll_logic.h - Lab 4: how many labels are left on a roll, judged by weight.
// Pure maths, no Arduino calls, so it can be tested on a normal computer (see ../tests).
#pragma once

// Illustrative roll: empty core 0.10 kg, full roll 1.50 kg holding 2000 labels.
const float CORE_KG     = 0.10f;
const float FULL_KG     = 1.50f;
const int   LABELS_FULL = 2000;

// Stock levels with hysteresis (like Lab 1) so the lights do not flicker.
const float REORDER_PCT  = 20.0f;   // below this: order more
const float CRITICAL_PCT = 5.0f;    // below this: the roll is about to run out
const float HYST_PCT     = 2.0f;    // must recover by this much to clear a level

enum Level { LV_OK = 0, LV_REORDER = 1, LV_CRITICAL = 2 };
enum Check { CHK_OK = 0, CHK_NO_ROLL, CHK_TOO_HEAVY };

// "Is the reading believable?" - never guess when the scale is empty or overloaded.
inline Check checkReading(float grossKg) {
  if (grossKg < CORE_KG * 0.5f) return CHK_NO_ROLL;        // nothing (or only dust) on the scale
  if (grossKg > FULL_KG + 0.30f) return CHK_TOO_HEAVY;     // heavier than a full roll: wrong item?
  return CHK_OK;
}

inline float percentLeft(float grossKg) {
  float net = grossKg - CORE_KG;
  if (net < 0) net = 0;
  float pct = 100.0f * net / (FULL_KG - CORE_KG);
  if (pct > 100.0f) pct = 100.0f;
  return pct;
}

inline int labelsLeft(float grossKg) {
  return (int)(percentLeft(grossKg) / 100.0f * LABELS_FULL + 0.5f);
}

inline Level updateLevel(Level lv, float pct) {
  for (int i = 0; i < 2; i++) {                // two passes so a sudden big drop reaches CRITICAL
    if (lv == LV_OK && pct < REORDER_PCT) lv = LV_REORDER;
    else if (lv == LV_REORDER && pct < CRITICAL_PCT) lv = LV_CRITICAL;
    else if (lv == LV_CRITICAL && pct > CRITICAL_PCT + HYST_PCT) lv = LV_REORDER;
    else if (lv == LV_REORDER && pct > REORDER_PCT + HYST_PCT) lv = LV_OK;
  }
  return lv;
}
