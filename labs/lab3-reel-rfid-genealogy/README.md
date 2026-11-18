# Lab 3: Reel and roll tracking with RFID

> Illustrative scenario, simulated, invented numbers. Not a real machine.

**Story:** components arrive on reels and trays. Each has an RFID tag. A reader only reads the tag ID. A small ledger decides what the ID means and records receive, put-away and issue-to-line movements.

**Topics covered:** Module 5 (5.1 component inventory, 5.3 receiving and putaway, 5.4 traceability)

## Circuit
- MFRC522 RFID reader (SPI), three push buttons (receive, put away, issue to line), green OK LED, red REJECTED LED
- Pins: RFID SDA GPIO5, RST GPIO21, SCK GPIO18, MOSI GPIO23, MISO GPIO19; buttons GPIO25, 26, 27; LEDs GPIO2, GPIO4
- Library: MFRC522

## How to run
Follow `../HOW-TO-OPEN-A-LAB-IN-WOKWI.md`. Files: `sketch.ino`, the `*_logic.h` header (if present), `diagram.json`.

## What to do
1. Press an action button, then click the reader, pick a card and press Tap.\n2. Blue card = printhead tray, green = main boards, yellow = ribbon rolls. Red is unknown.\n3. Receive then put away the blue card.\n4. Try to issue the yellow card before it was received.\n5. Tap a card twice quickly.

## What you should see (example)
```
RECEIVE tag 01:02:03:04 -> OK: PH-203 lot LOT-PH-88 qty 20 now RECEIVED @ RECEIVING-DOCK
ISSUE TO LINE tag 55:66:77:88 -> REJECTED: wrong order
RECEIVE tag AA:BB:CC:DD -> REJECTED: tag not in the component list
```

## What-if changes to try
Add a fourth part to `ledger_logic.h` (for example a label-roll core). Then ask: which printers contain printhead lot LOT-PH-88? (extension: store the printer serial when issuing).

## Questions to think about
- What three kinds of mistake did the ledger refuse?
- Why is the reader not the place that knows what a tag means?
- How would you answer a recall question with this data?

## Limits
A simple rule in a simulator. Not artificial intelligence, not a safety system, not a real diagnosis.
