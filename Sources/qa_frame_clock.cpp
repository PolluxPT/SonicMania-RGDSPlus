#include "sonic-mania/dependencies/RSDKv5/rgds-frame-clock.hpp"
#include <cassert>
#include <cstdio>
int main() {
 const uint64_t p=1000000/60;
 assert(rgdsAdvanceFrameDeadline(0,123,p)==123+p);
 assert(rgdsAdvanceFrameDeadline(100000,100000,p)==100000+p);
 assert(rgdsAdvanceFrameDeadline(100000,100020,p)==100000+p);
 assert(rgdsAdvanceFrameDeadline(100000,100000+p,p)==100000+2*p);
 assert(rgdsAdvanceFrameDeadline(100000,900000,p)==900000+p);
 uint64_t d=100000;
 for(int i=0;i<100000;i++) {
   uint64_t now=d+(i%200);
   auto next=rgdsAdvanceFrameDeadline(d,now,p);
   assert(next==d+p && next>now);
   d=next;
 }
 assert(d==100000+100000*p);
 puts("deadline clock: drift, jitter, stalls, no catch-up: PASS");
}
