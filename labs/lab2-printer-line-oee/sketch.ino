/*
  Lab 2 - Printer final-assembly line: production monitoring and OEE
  (ILLUSTRATIVE line "FA-01". Simulated. Not real data.)

  You play the line supervisor. Three buttons:
    GREEN  = RUN / STOP the line (toggle)
    BLUE   = one printer PASSED end-of-line test
    YELLOW = one printer FAILED test (sent to rework)

  Workshop topic: 4.4 real-time production monitoring.

  Simulated time: 1 real second = 1 simulated minute.
  Planned production time = 60 minutes (one hour = 60 real seconds).
  Ideal cycle time = 2 minutes per printer (so the best possible is 30 printers in a full hour).
  Printers can ONLY be counted while the line is RUNNING.

  OEE = Availability x Performance x Quality
    Availability = Run time / Planned time
    Performance  = (Ideal cycle time x Total count) / Run time
    Quality      = Good count / Total count

  Idea adapted from DATATWEETS intro-to-iot-workshop lab-1 (buttons) and lab-7 (events).
  The OEE logic is new and lives in oee_logic.h.
*/
#include "oee_logic.h"

const int RUN_BTN   = 13;   // toggle run/stop
const int GOOD_BTN  = 33;   // good part
const int FAIL_BTN = 14;   // scrap part
const int RUN_LED   = 2;    // green = running
const int STOP_LED  = 4;    // red   = stopped

const float PLANNED_MIN     = 60.0;  // simulated minutes in this shift slice
const float IDEAL_CYCLE_MIN = 2.0;   // minutes per printer (change me in What-if 3!)
const unsigned long MS_PER_SIM_MIN = 1000;  // 1 real second = 1 simulated minute

bool running = false;
bool finished = false;
unsigned long lastTick = 0;
float simMin = 0, runMin = 0;
int totalCount = 0, goodCount = 0;
unsigned long lastReport = 0;

// button helper: true once per press
bool pressed(int pin, bool &prev) {
  bool now = (digitalRead(pin) == LOW);
  bool edge = now && !prev;
  prev = now;
  return edge;
}
bool pRun = false, pGood = false, pFail = false;

void report(const char *title) {
  OeeInput in = {simMin, runMin, IDEAL_CYCLE_MIN, totalCount, goodCount};
  OeeResult r = computeOee(in);
  Serial.println("------------------------------------");
  Serial.println(title);
  Serial.print("Planned "); Serial.print(simMin, 0); Serial.print(" min | Run ");
  Serial.print(runMin, 0); Serial.print(" min | Stopped "); Serial.print(simMin - runMin, 0);
  Serial.print(" min | Total "); Serial.print(totalCount);
  Serial.print(" | Good "); Serial.print(goodCount);
  Serial.print(" | Fail "); Serial.println(totalCount - goodCount);
  if (!r.valid || simMin <= 0) { Serial.println("OEE: not enough data yet"); return; }
  Serial.print("Availability "); Serial.print(r.availability * 100, 1); Serial.println(" %");
  Serial.print("Performance  "); Serial.print(r.performance  * 100, 1); Serial.println(" %");
  Serial.print("Quality      "); Serial.print(r.quality      * 100, 1); Serial.println(" %");
  Serial.print("OEE          "); Serial.print(r.oee          * 100, 1); Serial.println(" %");
  if (r.perfOver100)
    Serial.println("WARNING: Performance > 100%. Printers were made faster than the ideal cycle allows - check the counts or the ideal cycle time.");
}

void setup() {
  Serial.begin(115200);
  pinMode(RUN_BTN, INPUT_PULLUP);
  pinMode(GOOD_BTN, INPUT_PULLUP);
  pinMode(FAIL_BTN, INPUT_PULLUP);
  pinMode(RUN_LED, OUTPUT);
  pinMode(STOP_LED, OUTPUT);
  digitalWrite(STOP_LED, HIGH);
  Serial.println("=== Lab 2: Line FA-01 production monitor (simulated) ===");
  Serial.println("Line is STOPPED. Press GREEN to start. 1 real second = 1 simulated minute.");
  Serial.println("A 60-minute run takes 60 real seconds. To reset: press Stop then Play on the simulator.");
  lastTick = millis();
}

void loop() {
  unsigned long now = millis();

  if (!finished) {
    // advance simulated time
    while (now - lastTick >= MS_PER_SIM_MIN) {
      lastTick += MS_PER_SIM_MIN;
      simMin += 1;
      if (running) runMin += 1;
      if (simMin >= PLANNED_MIN) { finished = true; break; }
    }
  }

  if (pressed(RUN_BTN, pRun) && !finished) {
    running = !running;
    Serial.print("["); Serial.print(simMin, 0); Serial.print(" min] Line ");
    Serial.println(running ? "RUNNING" : "STOPPED (downtime is being counted)");
  }
  if (pressed(GOOD_BTN, pGood)) {
    if (running && !finished) { totalCount++; goodCount++; Serial.println("+1 PASS (good printer)"); }
    else Serial.println("Ignored: line is stopped (or shift is over) - no printers can be made.");
  }
  if (pressed(FAIL_BTN, pFail)) {
    if (running && !finished) { totalCount++; Serial.println("+1 FAIL (rework)"); }
    else Serial.println("Ignored: line is stopped (or shift is over) - no printers can be made.");
  }

  digitalWrite(RUN_LED,  running ? HIGH : LOW);
  digitalWrite(STOP_LED, running ? LOW : HIGH);

  if (!finished && simMin - lastReport >= 10) { lastReport = (unsigned long)simMin; report("10-minute update"); }
  if (finished) {
    static bool done = false;
    if (!done) { done = true; running = false; report("FINAL REPORT - 60 planned minutes complete"); }
  }
  delay(5);   // poll buttons often so short clicks are never missed
}
