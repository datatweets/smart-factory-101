// ledger_logic.h - Lab 3: a tiny LOCAL component ledger (teaching toy, NOT a WMS or MES).
// Each RFID tag is attached to ONE container (a box of parts). A container moves through
//   NEW -> RECEIVED -> STORED -> ISSUED (issued to the assembly line)
// Wrong order, unknown tags and double taps are rejected, never silently counted.
#pragma once
#include <string.h>

enum Action { ACT_RECEIVE = 0, ACT_PUTAWAY = 1, ACT_ISSUE = 2 };
enum Status { ST_NEW = 0, ST_RECEIVED = 1, ST_STORED = 2, ST_ISSUED = 3 };
enum Result { RES_OK = 0, RES_UNKNOWN_TAG, RES_DUPLICATE, RES_WRONG_ORDER };

struct Container {
  const char *uid;      // RFID tag UID, e.g. "01:02:03:04"
  const char *partId;   // illustrative material master
  const char *desc;
  const char *lot;
  int         qty;      // pieces in this container
  Status      status;
  const char *zone;     // where it is now
};

const int NUM_CONTAINERS = 3;
Container containers[NUM_CONTAINERS] = {
  {"01:02:03:04", "PH-203", "Printhead 203 dpi",      "LOT-PH-88", 20,  ST_NEW, "SUPPLIER"},
  {"11:22:33:44", "PCB-MAIN", "Main board",           "LOT-B-204", 50,  ST_NEW, "SUPPLIER"},
  {"55:66:77:88", "RIB-110", "Thermal ribbon 110 mm", "LOT-R-31",  100, ST_NEW, "SUPPLIER"}
};

inline int findContainer(const char *uid) {
  for (int i = 0; i < NUM_CONTAINERS; i++) if (strcmp(containers[i].uid, uid) == 0) return i;
  return -1;
}

// lastUid/lastAct/lastMs implement duplicate-tap protection (same tag + same action within window).
struct DupGuard { char uid[24]; int act; unsigned long ms; bool valid; };
const unsigned long DUP_WINDOW_MS = 3000;

inline Result applyEvent(const char *uid, Action act, unsigned long nowMs, DupGuard &g, int &idxOut) {
  idxOut = findContainer(uid);
  if (idxOut < 0) return RES_UNKNOWN_TAG;
  if (g.valid && strcmp(g.uid, uid) == 0 && g.act == (int)act && (nowMs - g.ms) < DUP_WINDOW_MS)
    return RES_DUPLICATE;
  Container &c = containers[idxOut];
  Status need = (act == ACT_RECEIVE) ? ST_NEW : (act == ACT_PUTAWAY) ? ST_RECEIVED : ST_STORED;
  if (c.status != need) return RES_WRONG_ORDER;   // e.g. issuing a reel that was never stored
  if (act == ACT_RECEIVE)  { c.status = ST_RECEIVED; c.zone = "RECEIVING-DOCK"; }
  if (act == ACT_PUTAWAY)  { c.status = ST_STORED;   c.zone = "RACK-A1"; }
  if (act == ACT_ISSUE) { c.status = ST_ISSUED;  c.zone = "LINE-FA01"; }
  strncpy(g.uid, uid, sizeof(g.uid) - 1); g.uid[sizeof(g.uid) - 1] = 0;
  g.act = (int)act; g.ms = nowMs; g.valid = true;
  return RES_OK;
}

// Stock "in stores" counts only STORED containers. Quantity is derived, never typed in twice.
inline int piecesInStock(const char *partId) {
  int total = 0;
  for (int i = 0; i < NUM_CONTAINERS; i++)
    if (containers[i].status == ST_STORED && strcmp(containers[i].partId, partId) == 0) total += containers[i].qty;
  return total;
}
