# Lab 4: Label-roll scale

> Illustrative scenario, simulated, invented numbers. Not a real machine.

**Story:** a roll of label stock sits on a scale. Its weight tells you how many labels are left, so nobody has to unwind the roll. Lights show OK, reorder (below 20 per cent) and critical (below 5 per cent).

**Topics covered:** Module 5 (5.2 materials management) and 6.3 indirect materials (smart bin)

## Circuit
- HX711 load-cell amplifier (5 kg), green, amber and red LEDs, buzzer
- Pins: HX711 DT on GPIO16, SCK on GPIO17; green LED GPIO2, amber GPIO15, red GPIO4, buzzer GPIO18; HX711 powered from the 5V pin
- Library: HX711 Arduino Library

## How to run
Follow `../HOW-TO-OPEN-A-LAB-IN-WOKWI.md`. Files: `sketch.ino`, the `*_logic.h` header (if present), `diagram.json`.

## What to do
1. Click the HX711 board and drag LOAD.\n2. Try 1.50, 0.80, 0.36, 0.40, 0.45, 0.15, 3.00 and 0 kg.\n3. Watch the lights and the labels-left estimate.

## What you should see (example)
```
Gross 1.50 kg | labels left ~2000 (100.0%) | OK
Gross 0.36 kg | labels left ~371 (18.5%) | REORDER  <-- level changed
Gross 0.15 kg | labels left ~71 (3.6%) | CRITICAL
Gross 3.00 kg | CHECK: heavier than a full roll. Wrong item on the scale?
```

## What-if changes to try
Change `FULL_KG` and `LABELS_FULL` in `roll_logic.h` to model a different roll. Change `REORDER_PCT` to 30: what happens to how early the amber light comes on?

## Questions to think about
- Why does the amber light stay on at 0.40 kg?
- Why does the sketch refuse to guess when the scale is empty or overloaded?
- Which of your own consumables could use a scale like this?

## Limits
A simple rule in a simulator. Not artificial intelligence, not a safety system, not a real diagnosis.
