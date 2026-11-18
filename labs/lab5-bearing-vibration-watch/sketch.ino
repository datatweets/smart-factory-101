/*
  Lab 5 - Bearing vibration watch on a die-cutter (condition-based maintenance)
  (ILLUSTRATIVE machine and limits. Simulated sensor. Not a real machine or a real diagnosis.)

  Story: an accelerometer is stuck on the motor housing of a converting machine.
  After startup the sketch learns what "healthy" looks like (the baseline). Then it
  reports how far the reading moves away from it, in milli-g (mg).
  In real life vibration changes on its own. In the simulator YOU make it change:
  click the MPU6050 and move its accelerometer sliders (X, Y, Z).

  Buttons:
    BLUE   = RECORD WEEK  (save today's reading as the next weekly point, up to 10)
    YELLOW = RE-LEARN baseline (do this after maintenance)

  Lights: green = normal, amber = watch (130 mg or a rising trend), red + buzzer = alert (280 mg).

  Workshop topic: 6.2 managing capital assets. A threshold rule and a trend rule, not AI.
*/
#include <Wire.h>
#include "vib_logic.h"

const int SDA_PIN = 21, SCL_PIN = 22;
const int BTN_WEEK = 25, BTN_LEARN = 26;
const int LED_GREEN = 2, LED_AMBER = 15, LED_RED = 4, BUZZER = 18;
const uint8_t MPU = 0x68;

Vec baseline = {0, 0, 1};
Level level = LV_NORMAL;
Trend trend = {{0}, 0};
bool trendWarn = false;

bool readAccel(Vec &a) {
  Wire.beginTransmission(MPU); Wire.write(0x3B);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom((int)MPU, 6) != 6) return false;
  int16_t x = (Wire.read() << 8) | Wire.read();
  int16_t y = (Wire.read() << 8) | Wire.read();
  int16_t z = (Wire.read() << 8) | Wire.read();
  a.x = x / 16384.0f; a.y = y / 16384.0f; a.z = z / 16384.0f;   // +/-2 g range
  return true;
}

void learnBaseline() {
  Serial.println("Learning baseline for 3 seconds - keep the sliders where the machine is healthy...");
  Vec sum = {0, 0, 0}; int n = 0;
  for (int i = 0; i < 15; i++) { Vec a; if (readAccel(a)) { sum.x += a.x; sum.y += a.y; sum.z += a.z; n++; } delay(200); }
  if (n == 0) { Serial.println("SENSOR ERROR - no readings, check wiring"); return; }
  baseline = {sum.x / n, sum.y / n, sum.z / n};
  level = LV_NORMAL; trend.n = 0; trendWarn = false;
  Serial.print("Baseline saved (g): "); Serial.print(baseline.x, 2); Serial.print(", ");
  Serial.print(baseline.y, 2); Serial.print(", "); Serial.println(baseline.z, 2);
}

void showLights() {
  bool alert = (level == LV_ALERT);
  digitalWrite(LED_GREEN, (level == LV_NORMAL && !trendWarn) ? HIGH : LOW);
  digitalWrite(LED_AMBER, (level == LV_WATCH || (trendWarn && level != LV_ALERT)) ? HIGH : LOW);
  digitalWrite(LED_RED, alert ? HIGH : LOW);
  digitalWrite(BUZZER, alert ? HIGH : LOW);
}

void printWeeks() {
  Serial.print("Weekly log (mg): ");
  for (int i = 0; i < trend.n; i++) { Serial.print(trend.v[i], 0); Serial.print(i + 1 < trend.n ? ", " : ""); }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_WEEK, INPUT_PULLUP); pinMode(BTN_LEARN, INPUT_PULLUP);
  pinMode(LED_GREEN, OUTPUT); pinMode(LED_AMBER, OUTPUT); pinMode(LED_RED, OUTPUT); pinMode(BUZZER, OUTPUT);
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.beginTransmission(MPU); Wire.write(0x6B); Wire.write(0); Wire.endTransmission();   // wake the sensor
  Serial.println("=== Lab 5: Bearing vibration watch (simulated) ===");
  learnBaseline();
  Serial.println("Now move the MPU6050 accelerometer sliders. BLUE = record week, YELLOW = re-learn baseline.");
}

unsigned long lastPrint = 0;
bool pWeek = false, pLearn = false;
// true once per press (edge), so holding a button does not record twice
bool pressed(int pin, bool &prev) { bool now = (digitalRead(pin) == LOW); bool e = now && !prev; prev = now; return e; }

void loop() {
  if (pressed(BTN_LEARN, pLearn)) { learnBaseline(); }
  if (pressed(BTN_WEEK, pWeek)) {
    Vec a; float mg = 0;
    if (readAccel(a)) mg = deviationMg(a, baseline);
    if (addWeek(trend, mg)) {
      trendWarn = rising(trend);
      Serial.print(">> Week "); Serial.print(trend.n); Serial.print(" recorded: "); Serial.print(mg, 0); Serial.println(" mg");
      printWeeks();
      if (trendWarn) Serial.println("!! RISING TREND over 3 weeks: plan maintenance now, even if no limit is crossed.");
    } else Serial.println(">> Weekly log is full (10 weeks). Press YELLOW to start again.");
  }

  if (millis() - lastPrint >= 1000) {
    lastPrint = millis();
    Vec a;
    if (!readAccel(a)) { Serial.println("SENSOR ERROR - no reading"); return; }
    float mg = deviationMg(a, baseline);
    Level before = level;
    level = updateLevel(level, mg);
    showLights();
    Serial.print("Vibration "); Serial.print(mg, 0); Serial.print(" mg from baseline | ");
    Serial.print(level == LV_NORMAL ? "NORMAL" : level == LV_WATCH ? "WATCH" : "ALERT");
    if (trendWarn) Serial.print(" | TREND RISING");
    if (level != before) Serial.print("  <-- level changed");
    Serial.println();
  }
  delay(5);   // poll the buttons often so short presses are not missed
}
