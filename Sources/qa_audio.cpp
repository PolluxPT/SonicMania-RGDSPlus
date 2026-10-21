#include "sonic-mania/dependencies/RSDKv5/rgds-audio-utils.hpp"
#include <cassert>
#include <limits>
#include <vector>
#include <cstdio>
int main(){
 std::vector<float>source(4096),out;
 for(int i=0;i<4096;i++)source[i]=2.8f*std::sin(i*0.071f);
 for(int level=0;level<=10;level++){
  float t=level/10.0f,gain=0.35f*t*t;out=source;auto r=rgdsFinishAudio(out.data(),out.size(),gain);
  assert(!r.clipped&&!r.invalid);
  for(int i=0;i<4096;i++)assert(out[i]==source[i]*gain);
 }
 float edge[]={std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity(),std::numeric_limits<float>::quiet_NaN(),20,-20,0.5f};
 auto r=rgdsFinishAudio(edge,6,0.35f);assert(r.invalid==3&&r.clipped==2&&r.peak==7.0f);
 assert(edge[0]==0&&edge[1]==0&&edge[2]==0&&edge[3]==1&&edge[4]==-1&&edge[5]==0.175f);
 puts("PASS: unchanged finite PCM at every stock volume level, exact mute, gain curve, headroom, nonfinite rejection, peak clipping");
}
