# smart-factory-101

Hands-on introduction to **Industry 4.0 smart manufacturing**: a slide deck, three paper exercises and five small simulator labs you can run in your browser. No hardware and no coding experience needed.

## What is inside

| Folder | Contents |
|---|---|
| [`slides/`](slides) | The full course deck as a PDF (eight modules, 33 topics) |
| [`handouts/`](handouts) | Three one-page exercises: score a process, draw the path of one scan, build a 90-day plan and a quick ROI |
| [`labs/`](labs) | Five ESP32 simulations for [Wokwi](https://wokwi.com), each with code, wiring, a README and a step-by-step PDF guide |

## The five labs

| Lab | What you build | Idea it teaches |
|---|---|---|
| [1](labs/lab1-printhead-cooling-guard) | Temperature guard with a fan and an alarm | Sense, decide, act; hysteresis |
| [2](labs/lab2-printer-line-oee) | Production line monitor | OEE = availability x performance x quality |
| [3](labs/lab3-reel-rfid-genealogy) | RFID tag ledger for receiving and stock | Traceability; refusing mistakes |
| [4](labs/lab4-label-roll-scale) | Label-roll scale with reorder lights | Materials management by weight |
| [5](labs/lab5-bearing-vibration-watch) | Vibration watch with a trend rule | Condition-based maintenance |

## How to start

1. Open a lab folder and read its `Lab*-Guide.pdf`. It walks through the setup with screenshots.
2. Go to [wokwi.com/projects/new/esp32](https://wokwi.com/projects/new/esp32), then paste `diagram.json`, `sketch.ino` and the header file. [`labs/HOW-TO-OPEN-A-LAB-IN-WOKWI.md`](labs/HOW-TO-OPEN-A-LAB-IN-WOKWI.md) has the five steps.
3. Press Play and follow the guide.

The labs use invented numbers and simple threshold rules. They are teaching models, not real machines, not artificial intelligence and not safety systems. The free Wokwi tier is for personal, non-commercial use; check Wokwi's terms for classroom or commercial use.

`labs/tests/` holds small C++ tests for the decision logic of each lab. For example: `clang++ -std=c++17 -Ilabs/lab1-printhead-cooling-guard labs/tests/test_lab1.cpp -o t && ./t`.

## Licences

- Code in `labs/`: MIT (see `LICENSE`). Labs 1 to 3 are adapted from the MIT-licensed *intro-to-iot-workshop* labs by DATATWEETS.
- Slides, handouts and guides: CC BY-SA 4.0. Photographs in the slides come from Wikimedia Commons under their own open licences, credited on the slides.
