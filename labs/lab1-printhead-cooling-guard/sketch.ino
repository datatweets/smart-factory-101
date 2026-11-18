/*
  Lab 1 - Printhead burn-in rack: temperature, alarm and action
  (ILLUSTRATIVE scenario. Simulated sensor. Not a real machine.)

  Story: finished thermal printers are run on a burn-in rack, printing test
  labels for hours. Someone walks past once an hour and reads the chamber
  thermometer. Here an ESP32 reads the temperature every 2 seconds, switches a
  cooling fan (relay + blue LED) and raises an alarm (red LED + buzzer).

  Workshop topics: 1.1 sense-decide-act, 3.1 reasons for sensors.
  Adapted from the DATATWEETS intro-to-iot-workshop labs (MIT licence).
  The relay is only a SIMULATED fan. This is a simple rule, not AI, and not a safety system.
*/
#include "DHTesp.h"
#include "guard_logic.h"

const int DHT_PIN    = 15;  // temperature sensor
const int RELAY_PIN  = 5;   // simulated cooling fan
const int ALARM_LED  = 4;   // red alarm light
const int BUZZER_PIN = 18;  // alarm sound

DHTesp dht;
GuardState state = {false, false};

void setup() {
  Serial.begin(115200);
  dht.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(ALARM_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.println("=== Lab 1: Burn-in rack temperature guard (simulated) ===");
  Serial.println("Fan ON above 30 C, OFF below 28 C. Alarm at 35 C or more, clears below 33 C.");
  Serial.println("Click the DHT22 and change its temperature.");
}

void loop() {
  TempAndHumidity d = dht.getTempAndHumidity();
  float t = d.temperature;

  if (isnan(t)) {                       // sensor problem: say so, do not guess
    Serial.println("SENSOR ERROR - no reading");
    delay(2000);
    return;
  }

  GuardState before = state;
  state = updateGuard(state, t);

  digitalWrite(RELAY_PIN,  state.fanOn   ? HIGH : LOW);
  digitalWrite(ALARM_LED,  state.alarmOn ? HIGH : LOW);
  digitalWrite(BUZZER_PIN, state.alarmOn ? HIGH : LOW);

  Serial.print("Temp ");  Serial.print(t, 1);
  Serial.print(" C | Fan "); Serial.print(state.fanOn ? "ON " : "OFF");
  Serial.print(" | Alarm "); Serial.print(state.alarmOn ? "ON " : "OFF");
  if (state.fanOn   != before.fanOn)   Serial.print("  <-- fan changed");
  if (state.alarmOn != before.alarmOn) Serial.print("  <-- alarm changed");
  Serial.println();
  delay(2000);
}
