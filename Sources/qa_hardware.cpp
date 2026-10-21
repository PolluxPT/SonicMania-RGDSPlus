#include "sonic-mania/dependencies/RSDKv5/rgds-hardware.cpp"
#include <cassert>
static int readNode(){FILE*f=fopen(lowerNode,"r");int n=-1;assert(f&&fscanf(f,"%d",&n)==1);fclose(f);return n;}
static void writeNode(int n){FILE*f=fopen(lowerNode,"w");assert(f);fprintf(f,"%d\n",n);assert(!fclose(f));}
int main(){
 SDL_Init(SDL_INIT_TIMER);
 lowerNode="/tmp/sonicmania-screen-qa";writeNode(73);
 unsetenv("RGDS_STOCK");firstFrame=SDL_GetTicks()-500;rgdsScreenFrame();assert(readNode()==73);
 setenv("RGDS_STOCK","1",1);setenv("RGDS_LOWER_SAVED","57",1);
 firstFrame=SDL_GetTicks()-500;rgdsScreenFrame();assert(readNode()==0&&lowerSaved==57);
 writeNode(91);lastScreenCheck=SDL_GetTicks()-1001;rgdsScreenFrame();assert(readNode()==0);
 rgdsRestoreLower();assert(readNode()==57);
 lowerSaved=-1;firstFrame=SDL_GetTicks()-500;lastScreenCheck=0;unsetenv("RGDS_LOWER_SAVED");writeNode(29);
 rgdsScreenFrame();assert(readNode()==0);rgdsRestoreLower();assert(readNode()==29);
 rgdsInstallSignals();assert(!rgdsQuitRequested());raise(SIGTERM);assert(rgdsQuitRequested());
 unlink(lowerNode);SDL_Quit();puts("PASS: stock-only backlight control, parent snapshot, re-dimming, exact restoration, graceful signal flag");
}
