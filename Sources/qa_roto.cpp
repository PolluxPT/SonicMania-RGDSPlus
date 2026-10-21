#include "sonic-mania/dependencies/RSDKv5/rgds-render-utils.hpp"
#include <random>
#include <cassert>
#include <cstdio>
#include <chrono>
void reference(uint16_t *out,int count,uint32_t x,uint32_t y,uint32_t dx,uint32_t dy,unsigned shift,uint32_t mx,uint32_t my,const uint16_t *layout,const uint8_t *pixels,const uint16_t *pal){
 for(int i=0;i<count;i++){int32_t px=(int32_t)x,py=(int32_t)y;uint16_t tile=layout[(mx&(px>>20))+((my&(py>>20))<<shift)]&0xfff;uint8_t idx=pixels[16*(((py>>16)&15)+16*tile)+((px>>16)&15)];if(idx)out[i]=pal[idx];x+=dx;y+=dy;}
}
int main(){std::mt19937 rng(456);std::vector<uint16_t>layout(128*128),pal(256),a(1024),b(1024);std::vector<uint8_t>pixels(4096*256);
 for(auto &x:layout)x=rng();for(auto &x:pal)x=rng();for(auto &x:pixels)x=rng()%17;
 for(int trial=0;trial<20000;trial++){
  unsigned shift=rng()%8,hy=rng()%8;uint32_t mx=(1u<<shift)-1,my=(1u<<hy)-1,x=rng(),y=rng(),dx=rng(),dy=rng();int n=rng()%1025;
  for(int i=0;i<1024;i++)a[i]=b[i]=rng();reference(a.data(),n,x,y,dx,dy,shift,mx,my,layout.data(),pixels.data(),pal.data());rgdsRotoRow(b.data(),n,x,y,dx,dy,shift,mx,my,layout.data(),pixels.data(),pal.data());assert(a==b);
 }
 puts("PASS: 20000 randomized rows, negative/wrapped coords, transparency, clipping lengths, guard pixels");
}
