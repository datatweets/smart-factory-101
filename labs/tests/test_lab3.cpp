#include <cstdio>
#include "ledger_logic.h"
int fails=0;
#define CHECK(c) do{ if(!(c)){ printf("FAIL line %d: %s\n",__LINE__,#c); fails++; } }while(0)
int main(){
  DupGuard g={"",0,0,false}; int i;
  CHECK(applyEvent("AA:BB:CC:DD",ACT_RECEIVE,100,g,i)==RES_UNKNOWN_TAG);
  CHECK(applyEvent("01:02:03:04",ACT_PUTAWAY,200,g,i)==RES_WRONG_ORDER);   // not received yet
  CHECK(applyEvent("01:02:03:04",ACT_RECEIVE,300,g,i)==RES_OK);
  CHECK(applyEvent("01:02:03:04",ACT_RECEIVE,1000,g,i)==RES_DUPLICATE);   // double tap within 3 s
  CHECK(applyEvent("01:02:03:04",ACT_PUTAWAY,5000,g,i)==RES_OK);
  CHECK(piecesInStock("PH-203")==20); CHECK(piecesInStock("PCB-MAIN")==0);
  CHECK(applyEvent("11:22:33:44",ACT_RECEIVE,6000,g,i)==RES_OK);
  CHECK(applyEvent("11:22:33:44",ACT_PUTAWAY,10000,g,i)==RES_OK);
  CHECK(piecesInStock("PCB-MAIN")==50);
  CHECK(applyEvent("01:02:03:04",ACT_ISSUE,20000,g,i)==RES_OK);           // issued to line leaves stock
  CHECK(piecesInStock("PH-203")==0);
  CHECK(applyEvent("55:66:77:88",ACT_ISSUE,30000,g,i)==RES_WRONG_ORDER);   // never received or stored
  printf(fails?"LAB3 FAILED\n":"LAB3 PASS\n"); return fails;
}
