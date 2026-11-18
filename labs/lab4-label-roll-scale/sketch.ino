/*
  Lab 4 - Label roll scale: how much stock is left, and when to reorder
  (ILLUSTRATIVE roll and numbers. Simulated load cell. Not a real product or process.)

  Story: a roll of label stock sits on a scale at a converting machine or in the
  stores. Weight tells us how much is left, so nobody has to unwind the roll to
  count. A reorder light comes on at 20 percent; a critical light and buzzer at 5 percent.

  How to play: click the green HX711 board in the picture and drag the LOAD slider
  (kilograms). 1.50 = full roll, 0.10 = empty core.

  Workshop topics: 5.2 materials management, 6.3 indirect materials (smart bin).
  The HX711 simulation reports 0-2100 raw units for the 5 kg load cell, so
  420 raw units = 1 kg.
*/
#include "HX711.h"
#include "roll_logic.h"

const int DT_PIN = 16, SCK_PIN = 17;
const int LED_GREEN = 2, LED_AMBER = 15, LED_RED = 4, BUZZER = 18;
const float RAW_PER_KG = 420.0f;

HX711 scale;
Level level = LV_OK;

void show(Level lv) {
  digitalWrite(LED_GREEN, lv == LV_OK ? HIGH : LOW);
  digitalWrite(LED_AMBER, lv == LV_REORDER ? HIGH : LOW);
  digitalWrite(LED_RED,   lv == LV_CRITICAL ? HIGH : LOW);
  digitalWrite(BUZZER,    lv == LV_CRITICAL ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_GREEN, OUTPUT); pinMode(LED_AMBER, OUTPUT); pinMode(LED_RED, OUTPUT); pinMode(BUZZER, OUTPUT);
  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale(RAW_PER_KG);
  Serial.println("=== Lab 4: Label roll scale (simulated) ===");
  Serial.println("Click the HX711 and drag LOAD. 1.50 kg = full roll (2000 labels), 0.10 kg = empty core.");
  Serial.println("Green = OK. Amber = reorder (below 20%). Red + buzzer = critical (below 5%).");
}

void loop() {
  if (!scale.is_ready()) { Serial.println("Scale not ready"); delay(500); return; }
  float kg = scale.get_units(3);

  Check c = checkReading(kg);
  Serial.print("Gross "); Serial.print(kg, 2); Serial.print(" kg | ");
  if (c == CHK_NO_ROLL)      { Serial.println("NO ROLL on the scale"); level = LV_OK; show(LV_OK); delay(1000); return; }
  if (c == CHK_TOO_HEAVY)    { Serial.println("CHECK: heavier than a full roll. Wrong item on the scale?"); delay(1000); return; }

  float pct = percentLeft(kg);
  Level before = level;
  level = updateLevel(level, pct);
  show(level);

  Serial.print("labels left ~"); Serial.print(labelsLeft(kg));
  Serial.print(" ("); Serial.print(pct, 1); Serial.print("%) | ");
  Serial.print(level == LV_OK ? "OK" : level == LV_REORDER ? "REORDER" : "CRITICAL");
  if (level != before) Serial.print("  <-- level changed");
  Serial.println();
  delay(1000);
}
