#include <cstdio>
#include <cmath>
#include "roll_logic.h"
int fails=0;
#define CHECK(c) do{ if(!(c)){ printf("FAIL line %d: %s\n",__LINE__,#c); fails++; } }while(0)
int main(){
  CHECK(labelsLeft(1.50f)==2000); CHECK(labelsLeft(0.10f)==0); CHECK(labelsLeft(0.80f)==1000);
  CHECK(labelsLeft(2.0f)==2000);                     // capped, never above a full roll
  CHECK(fabs(percentLeft(0.38f)-20.0f)<0.01f);       // 0.28 / 1.40 = 20 %
  CHECK(checkReading(0.0f)==CHK_NO_ROLL); CHECK(checkReading(1.2f)==CHK_OK); CHECK(checkReading(2.5f)==CHK_TOO_HEAVY);
  Level lv=LV_OK;
  lv=updateLevel(lv,50);  CHECK(lv==LV_OK);
  lv=updateLevel(lv,19);  CHECK(lv==LV_REORDER);
  lv=updateLevel(lv,21);  CHECK(lv==LV_REORDER);     // hysteresis: 21 is not enough to clear
  lv=updateLevel(lv,23);  CHECK(lv==LV_OK);
  lv=updateLevel(lv,3);   CHECK(lv==LV_CRITICAL);    // a sudden big drop jumps straight to critical
  lv=updateLevel(lv,6);   CHECK(lv==LV_CRITICAL);
  lv=updateLevel(lv,8);   CHECK(lv==LV_REORDER);
  printf(fails?"LAB4 FAILED\n":"LAB4 PASS\n"); return fails;
}
