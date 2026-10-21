#include "sonic-mania/dependencies/RSDKv5/rgds-render-utils.hpp"
#include <cassert>
#include <chrono>
#include <cstdio>
#include <random>
#include <climits>
struct Face { int depth,index; };
void original(Face *a,int n){for(int i=1;i<n;i++){Face v=a[i];int j=i-1;while(j>=0&&a[j].depth<v.depth){a[j+1]=a[j];--j;}a[j+1]=v;}}
int main(){
 std::mt19937 r(1234);
 for(int n: {0,1,2,15,16,17,31,32,33,128,511,2048,16384})for(int trial=0;trial<12;trial++){
  std::vector<Face>a(n);for(int i=0;i<n;i++)a[i]={(int)(r()%21)-10,i};
  if(n>3){a[0].depth=INT_MIN;a[1].depth=INT_MAX;}
  auto b=a;original(a.data(),n);rgdsSortFaces(b.data(),n);for(int i=0;i<n;i++)assert(a[i].depth==b[i].depth&&a[i].index==b[i].index);
 }
 std::vector<Face>src(1024);for(int i=0;i<1024;i++)src[i]={(int)(r()%10000),i};auto a=src;
 auto begin=std::chrono::steady_clock::now();for(int i=0;i<2000;i++){a=src;original(a.data(),1024);}auto mid=std::chrono::steady_clock::now();
 for(int i=0;i<2000;i++){a=src;rgdsSortFaces(a.data(),1024);}auto end=std::chrono::steady_clock::now();
 printf("PASS: identical stable order incl. ties/extremes; 1024-face benchmark: old=%.2fms new=%.2fms\n",std::chrono::duration<double,std::milli>(mid-begin).count(),std::chrono::duration<double,std::milli>(end-mid).count());
}
