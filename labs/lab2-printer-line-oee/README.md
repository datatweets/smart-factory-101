# Lab 2: Printer final-assembly line OEE

> Illustrative scenario, simulated, invented numbers. Not a real machine.

**Story:** a final-assembly line is planned for 60 minutes. Stops, slow running and failed tests all reduce how much good output you get. OEE (Overall Equipment Effectiveness) puts the three together.

**Topics covered:** Module 4 (4.4 real-time production monitoring)

## Circuit
- Three push buttons (green run/stop, blue pass, yellow fail), green and red status LEDs
- Pins: run button GPIO13, pass button GPIO33, fail button GPIO14, run LED GPIO2, stop LED GPIO4
- Library: none

## How to run
Follow `../HOW-TO-OPEN-A-LAB-IN-WOKWI.md`. Files: `sketch.ino`, the `*_logic.h` header (if present), `diagram.json`.

## What to do
1. Press green to start. One real second equals one simulated minute.\n2. Press blue and yellow to count printers passed and failed.\n3. Stop the line twice for a while.\n4. Read the final report after 60 real seconds.

## What you should see (example)
```
[12 min] Line RUNNING
+1 PASS (good printer)
+1 FAIL (rework)
10-minute update with Availability, Performance, Quality and OEE
```

## What-if changes to try
Change `IDEAL_CYCLE_MIN` from 2.0 to 1.0 and run again. What happens to performance? Try pressing pass faster than the ideal rate: the program warns that performance is above 100 per cent.

## Questions to think about
- Which of the three factors moved most, and why?
- What would you do about a low availability?
- Why does the program ignore button presses while the line is stopped?

## Limits
A simple rule in a simulator. Not artificial intelligence, not a safety system, not a real diagnosis.
