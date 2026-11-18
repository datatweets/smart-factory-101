/*
  Lab 3 - Reel and roll tracking with RFID (simulated)
  (ILLUSTRATIVE parts and lots. Teaching toy - NOT a real warehouse system.)

  What the RFID reader does: it ONLY reads the tag's ID number (UID).
  What the little ledger in ledger_logic.h does: it decides what that ID MEANS
  (which component, which lot) and records the movement. In a real plant that
  second job belongs to a WMS or MES.

  Choose the action with the buttons, then tap a card on the reader:
    BLUE button   = RECEIVE        (reel / tray arrives at the receiving dock)
    YELLOW button = PUT AWAY       (moved into rack RACK-A1)
    RED button    = ISSUE TO LINE  (taken to final-assembly line FA-01)
  Cards in the simulator: Blue 01:02:03:04 = printhead tray, Green 11:22:33:44 = main boards,
  Yellow 55:66:77:88 = ribbon rolls. Red AA:BB:CC:DD is NOT in the list -> must be rejected.

  Workshop topics: 5.1 component inventory, 5.3 receiving/putaway, 5.4 traceability.
*/
#include <SPI.h>
#include <MFRC522.h>
#include "ledger_logic.h"

#define SS_PIN  5     // RFID "SDA" pin = SPI chip select
#define RST_PIN 21
const int BTN_RECEIVE = 25, BTN_PUTAWAY = 26, BTN_ISSUE = 27;
const int LED_OK = 2, LED_ERR = 4;

MFRC522 rfid(SS_PIN, RST_PIN);
Action currentAction = ACT_RECEIVE;
DupGuard guard = {"", 0, 0, false};

const char *actName(Action a) { return a == ACT_RECEIVE ? "RECEIVE" : a == ACT_PUTAWAY ? "PUT AWAY" : "ISSUE TO LINE"; }
const char *statusName(Status s) { return s == ST_NEW ? "NEW" : s == ST_RECEIVED ? "RECEIVED" : s == ST_STORED ? "STORED" : "ISSUED"; }

void flash(int pin) { digitalWrite(pin, HIGH); delay(300); digitalWrite(pin, LOW); }

void printLedger() {
  Serial.println("---- LOCAL LEDGER ----");
  for (int i = 0; i < NUM_CONTAINERS; i++) {
    Container &c = containers[i];
    Serial.print(c.partId); Serial.print(" lot "); Serial.print(c.lot);
    Serial.print(" qty "); Serial.print(c.qty);
    Serial.print(" | "); Serial.print(statusName(c.status));
    Serial.print(" @ "); Serial.println(c.zone);
  }
  Serial.print("In stock (STORED): printheads "); Serial.print(piecesInStock("PH-203"));
  Serial.print(", main boards "); Serial.print(piecesInStock("PCB-MAIN"));
  Serial.print(", ribbon rolls "); Serial.println(piecesInStock("RIB-110"));
  Serial.println("----------------------");
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_RECEIVE, INPUT_PULLUP); pinMode(BTN_PUTAWAY, INPUT_PULLUP); pinMode(BTN_ISSUE, INPUT_PULLUP);
  pinMode(LED_OK, OUTPUT); pinMode(LED_ERR, OUTPUT);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("=== Lab 3: reel and roll movements (simulated) ===");
  Serial.println("1) Press an action button  2) Click the reader, pick a card, press Tap.");
  Serial.print("Current action: "); Serial.println(actName(currentAction));
  printLedger();
}

void loop() {
  if (digitalRead(BTN_RECEIVE) == LOW)  { currentAction = ACT_RECEIVE;  Serial.println(">> Action: RECEIVE");  delay(250); }
  if (digitalRead(BTN_PUTAWAY) == LOW)  { currentAction = ACT_PUTAWAY;  Serial.println(">> Action: PUT AWAY"); delay(250); }
  if (digitalRead(BTN_ISSUE) == LOW) { currentAction = ACT_ISSUE; Serial.println(">> Action: ISSUE TO LINE"); delay(250); }

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  char uid[24]; uid[0] = 0;
  for (byte i = 0; i < rfid.uid.size; i++) {
    char part[4]; snprintf(part, sizeof(part), i ? ":%02X" : "%02X", rfid.uid.uidByte[i]);
    strcat(uid, part);
  }
  rfid.PICC_HaltA();

  int idx;
  Result r = applyEvent(uid, currentAction, millis(), guard, idx);
  Serial.print("[t="); Serial.print(millis() / 1000); Serial.print("s] "); Serial.print(actName(currentAction));
  Serial.print(" tag "); Serial.print(uid); Serial.print(" -> ");
  if (r == RES_OK) {
    Container &c = containers[idx];
    Serial.print("OK: "); Serial.print(c.partId); Serial.print(" lot "); Serial.print(c.lot);
    Serial.print(" qty "); Serial.print(c.qty); Serial.print(" now "); Serial.print(statusName(c.status));
    Serial.print(" @ "); Serial.println(c.zone);
    flash(LED_OK);
  } else if (r == RES_UNKNOWN_TAG) { Serial.println("REJECTED: tag not in the component list"); flash(LED_ERR); }
  else if (r == RES_DUPLICATE)     { Serial.println("IGNORED: duplicate tap (same tag, same action, <3 s)"); flash(LED_ERR); }
  else                             { Serial.println("REJECTED: wrong order (reel/tray is not in the right status for this action)"); flash(LED_ERR); }
  printLedger();
}
