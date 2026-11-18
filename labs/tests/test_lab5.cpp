#include <cstdio>
#include <cmath>
#include "vib_logic.h"
int fails=0;
#define CHECK(c) do{ if(!(c)){ printf("FAIL line %d: %s\n",__LINE__,#c); fails++; } }while(0)
int main(){
  Vec base={0,0,1};
  CHECK(fabs(deviationMg({0,0,1},base))<0.01f);
  CHECK(fabs(deviationMg({0.3f,0,1},base)-300.0f)<0.5f);
  CHECK(fabs(deviationMg({0.3f,0.4f,1},base)-500.0f)<0.5f);
  Level lv=LV_NORMAL;
  lv=updateLevel(lv,100); CHECK(lv==LV_NORMAL);
  lv=updateLevel(lv,150); CHECK(lv==LV_WATCH);
  lv=updateLevel(lv,120); CHECK(lv==LV_WATCH);       // hysteresis: needs to fall below 105
  lv=updateLevel(lv,100); CHECK(lv==LV_NORMAL);
  lv=updateLevel(lv,350); CHECK(lv==LV_ALERT);
  lv=updateLevel(lv,260); CHECK(lv==LV_ALERT);       // needs to fall below 255
  lv=updateLevel(lv,250); CHECK(lv==LV_WATCH);
  Trend t={{0},0};
  addWeek(t,100); addWeek(t,110); CHECK(!rising(t));         // too few points
  addWeek(t,120); CHECK(!rising(t));                         // only +20 mg: ignored as a wobble
  Trend u={{0},0}; addWeek(u,60); addWeek(u,90); addWeek(u,130); CHECK(rising(u));   // +70 mg over 3 weeks, all below the limit
  Trend w={{0},0}; addWeek(w,60); addWeek(w,130); addWeek(w,100); CHECK(!rising(w)); // not strictly rising
  Trend f={{0},0}; for(int i=0;i<MAX_WEEKS;i++) CHECK(addWeek(f,i)); CHECK(!addWeek(f,1)); // log full
  printf(fails?"LAB5 FAILED\n":"LAB5 PASS\n"); return fails;
}
