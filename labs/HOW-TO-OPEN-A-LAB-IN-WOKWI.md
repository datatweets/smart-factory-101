# How to open any lab in Wokwi (5 steps)

> Everything here is a **simulation** with invented data. It does not control a real machine.

You need a laptop, Chrome (or any modern browser) and a Wokwi login.

1. Go to **wokwi.com/projects/new/esp32**. You see a starter project with `sketch.ino` and `diagram.json` tabs.
2. Open the lab folder. For each file:
   - **`sketch.ino`** and **`diagram.json`** already exist: click the tab, select all (Ctrl/Cmd + A) and paste.
   - **`*_logic.h`** (a header file): click the small **down-arrow** at the right of the tabs, choose **New file...**, type the exact file name, then paste.
3. **Libraries:** open the **Library Manager** tab, click **+**, search the name in `libraries.txt` and click it.
   - Lab 1: `DHT sensor library for ESPx`
   - Lab 3: `MFRC522`
   - Lab 4: `HX711 Arduino Library`
   - Labs 2 and 5: none
4. Press the green **Play** button. The first start compiles the code (10 to 20 seconds). The text appears in the Serial Monitor under the circuit.
5. **To reset:** press the round restart button next to Stop.

## Handy controls

| Lab | How to change the input |
|---|---|
| 1 | Click the DHT22 sensor and drag its Temperature slider |
| 2 | Click the green, blue and yellow buttons (hold for a quarter of a second) |
| 3 | Click the RFID reader, pick a card, press Tap; choose the action with the three buttons |
| 4 | Click the HX711 board and drag the LOAD (kg) slider |
| 5 | Click the MPU6050 and drag its Acceleration X, Y, Z sliders; blue button = record week, yellow = re-learn baseline |

## If something goes wrong

- **Build failed with "was not declared in this scope":** a header file is missing or has a different name from the one in `sketch.ino`. Create it with the exact name.
- **The part list looks wrong:** paste `diagram.json` again, then press Play.
- **Nothing prints:** open the Serial Monitor panel under the circuit; check the baud rate is 115200.
- **The parts are off-screen:** drag the circuit canvas, or use the zoom controls in the simulator.
