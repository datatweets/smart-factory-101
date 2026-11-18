#include <cstdio>
#include <cmath>
#include "oee_logic.h"
int fails=0;
#define CHECK(c) do{ if(!(c)){ printf("FAIL line %d: %s\n",__LINE__,#c); fails++; } }while(0)
int main(){
  OeeResult r=computeOee({480,384,1.0f,330,310});          // the slide-41 worked example
  CHECK(r.valid); CHECK(fabs(r.availability-0.80f)<1e-4); CHECK(fabs(r.performance-330.0f/384)<1e-4);
  CHECK(fabs(r.quality-310.0f/330)<1e-4); CHECK(fabs(r.oee-310.0f/480)<1e-4); CHECK(!r.perfOver100);
  r=computeOee({60,60,2.0f,40,40}); CHECK(r.perfOver100);   // impossible speed is flagged, not hidden
  r=computeOee({0,0,2.0f,0,0}); CHECK(!r.valid);
  r=computeOee({60,30,2.0f,5,6}); CHECK(!r.valid);          // good > total
  printf(fails?"LAB2 FAILED\n":"LAB2 PASS\n"); return fails;
}
