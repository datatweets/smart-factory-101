# Lab 1: Printhead burn-in rack temperature guard

> Illustrative scenario, simulated, invented numbers. Not a real machine.

**Story:** finished thermal printers run test prints for hours on a burn-in rack. Someone reads the chamber thermometer once an hour. An ESP32 can read it every 2 seconds, switch a cooling fan and raise an alarm.

**Topics covered:** Module 3 (1.1 sense-decide-act, 3.1 reasons for sensors)

## Circuit
- DHT22 temperature sensor, relay module (as a simulated fan, blue LED), red alarm LED, buzzer
- Pins: DHT22 data on GPIO15, relay on GPIO5, alarm LED on GPIO4, buzzer on GPIO18
- Library: DHT sensor library for ESPx

## How to run
Follow `../HOW-TO-OPEN-A-LAB-IN-WOKWI.md`. Files: `sketch.ino`, the `*_logic.h` header (if present), `diagram.json`.

## What to do
1. Click the DHT22 and set the temperature to 24, 31, 36, 33, 31, 28.5, 27.5 (wait about 4 seconds each).\n2. Write down when the fan and the alarm change.

## What you should see (example)
```
Temp 24.0 C | Fan OFF | Alarm OFF
Temp 31.0 C | Fan ON  | Alarm OFF  <-- fan changed
Temp 36.0 C | Fan ON  | Alarm ON   <-- alarm changed
```

## What-if changes to try
Change `FAN_ON_C` from 30 to 27 in `guard_logic.h`. What happens to how often the fan runs? Set `FAN_OFF_C` equal to `FAN_ON_C` and hover around 30: what do you see?

## Questions to think about
- Why is the fan still on at 28.5 degrees?
- What can this rule not tell us (hint: tomorrow)?
- Who should own the alarm?

## Limits
A simple rule in a simulator. Not artificial intelligence, not a safety system, not a real diagnosis.
