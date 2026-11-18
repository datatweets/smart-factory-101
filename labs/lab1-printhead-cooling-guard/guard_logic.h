// guard_logic.h - Lab 1: threshold rule WITH hysteresis (pure logic, no Arduino calls).
// Because it has no Arduino calls it can be tested on a normal computer (see ../tests).
#pragma once

struct GuardState {
  bool fanOn;
  bool alarmOn;
};

// Fan:   switches ON above FAN_ON_C, switches OFF only below FAN_OFF_C.
// Alarm: switches ON at/above ALARM_ON_C, clears only below ALARM_OFF_C.
// The gap between the "on" and "off" levels is called hysteresis. It stops the
// fan flicking on and off when the reading hovers around a single number.
const float FAN_ON_C    = 30.0;
const float FAN_OFF_C   = 28.0;
const float ALARM_ON_C  = 35.0;
const float ALARM_OFF_C = 33.0;

inline GuardState updateGuard(GuardState s, float tempC) {
  if (!s.fanOn && tempC > FAN_ON_C)  s.fanOn = true;
  else if (s.fanOn && tempC < FAN_OFF_C) s.fanOn = false;

  if (!s.alarmOn && tempC >= ALARM_ON_C)  s.alarmOn = true;
  else if (s.alarmOn && tempC < ALARM_OFF_C) s.alarmOn = false;
  return s;
}
