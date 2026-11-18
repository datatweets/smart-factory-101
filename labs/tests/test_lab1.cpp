#include <cstdio>
#include "guard_logic.h"
int fails=0;
#define CHECK(c) do{ if(!(c)){ printf("FAIL line %d: %s\n",__LINE__,#c); fails++; } }while(0)
int main(){
  GuardState s={false,false};
  float seq[]={24,31,36,33,31,28.5f,27.5f};
  bool fan[]={0,1,1,1,1,1,0}, al[]={0,0,1,1,0,0,0};
  for(int i=0;i<7;i++){ s=updateGuard(s,seq[i]); CHECK(s.fanOn==fan[i]); CHECK(s.alarmOn==al[i]); }
  s={false,false}; s=updateGuard(s,30.0f); CHECK(!s.fanOn);     // exactly 30 is not "above 30"
  s=updateGuard(s,30.1f); CHECK(s.fanOn);
  printf(fails?"LAB1 FAILED\n":"LAB1 PASS\n"); return fails;
}
