# Lab 5: Bearing vibration watch

> Illustrative scenario, simulated, invented numbers. Not a real machine.

**Story:** an accelerometer is stuck on the motor housing of a converting machine. At start-up it learns the healthy baseline, then reports how far the reading moves away in milli-g. A limit rule and a trend rule both watch.

**Topics covered:** Module 6 (6.2 managing capital assets, condition-based maintenance)

## Circuit
- MPU6050 accelerometer (I2C), two push buttons (record week, re-learn baseline), green, amber and red LEDs, buzzer
- Pins: MPU6050 SDA GPIO21, SCL GPIO22; buttons GPIO25 and GPIO26; LEDs GPIO2, GPIO15, GPIO4; buzzer GPIO18
- Library: none (the sketch reads the sensor over I2C directly)

## How to run
Follow `../HOW-TO-OPEN-A-LAB-IN-WOKWI.md`. Files: `sketch.ino`, the `*_logic.h` header (if present), `diagram.json`.

## What to do
1. Press Play and wait 3 seconds while it learns the baseline.\n2. Click the MPU6050; move Accelerometer X slider to 0.00, press blue; 0.05, blue; 0.10, blue.\n3. See the trend warning before any limit is crossed.\n4. Move X to 0.35: alert. Move it back to 0.05.\n5. Press yellow after the pretend repair to learn a new baseline.

## What you should see (example)
```
Baseline saved (g): 0.00, 0.00, 1.00
>> Week 3 recorded: 150 mg
!! RISING TREND over 3 weeks: plan maintenance now
V 350 mg | ALERT | TREND RISING  <-- level changed
```

## What-if changes to try
Change `WATCH_MG` and `ALERT_MG` in `vib_logic.h`. Change `TREND_MIN_RISE_MG` to 100: does the trend rule still fire for 0, 50, 100?

## Questions to think about
- Why does the trend rule warn earlier than the limit?
- What does the baseline do?
- What would a real project need that this does not have (real vibration, filtering, history)?

## Limits
A simple rule in a simulator. Not artificial intelligence, not a safety system, not a real diagnosis.
