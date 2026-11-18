// oee_logic.h - Lab 2: OEE (Overall Equipment Effectiveness)
// Pure maths, no Arduino calls, so it can be tested on a normal computer.
// All times are in SIMULATED MINUTES (the sketch maps 1 real second = 1 simulated minute).
#pragma once

struct OeeInput {
  float plannedMin;    // planned production time so far (minutes)
  float runMin;        // time the machine was actually running (minutes)
  float idealCycleMin; // ideal time to make ONE part (minutes per part)
  int   totalCount;    // all parts made (good + scrap)
  int   goodCount;     // parts that passed inspection
};

struct OeeResult {
  float availability, performance, quality, oee;  // 0.0 - 1.0 (multiply by 100 for %)
  bool  valid;           // false if inputs make no sense
  bool  perfOver100;     // true if "performance" came out above 100% (a data warning)
};

inline OeeResult computeOee(OeeInput in) {
  OeeResult r = {0, 0, 0, 0, true, false};
  if (in.plannedMin <= 0 || in.idealCycleMin <= 0 || in.runMin < 0 ||
      in.totalCount < 0 || in.goodCount < 0 || in.goodCount > in.totalCount ||
      in.runMin > in.plannedMin + 0.0001f) {
    r.valid = false;
    return r;
  }
  r.availability = in.runMin / in.plannedMin;
  r.performance  = (in.runMin > 0) ? (in.idealCycleMin * in.totalCount) / in.runMin : 0;
  r.quality      = (in.totalCount > 0) ? (float)in.goodCount / in.totalCount : 0;
  r.perfOver100  = r.performance > 1.0001f;   // reported honestly, never silently capped
  r.oee          = r.availability * r.performance * r.quality;
  return r;
}
