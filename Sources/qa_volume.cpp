#include "sonic-mania/dependencies/RSDKv5/rgds-hardware.cpp"
#include <cmath>
#include <cassert>
int main() {
    unsetenv("RGDS_MANIA_PLUS"); assert(!rgdsPlusOptIn());
    setenv("RGDS_MANIA_PLUS","0",1); assert(!rgdsPlusOptIn());
    setenv("RGDS_MANIA_PLUS","1",1); assert(rgdsPlusOptIn());
    unsetenv("RGDS_MANIA_PLUS");
    struct Sample { int n; float expected; } samples[] = {{0,0.0f},{1,0.0035f},{2,0.014f},{5,0.0875f},{10,0.35f}};
    for (const auto &s : samples) { level=s.n; updateGain(); assert(std::fabs(rgdsAudioGain()-s.expected)<0.000001f); }
    SDL_Init(SDL_INIT_TIMER);
    setenv("RGDS_STOCK", "1", 1);
    volumeNode="/tmp/sonicmania-qa-volume";
    FILE *f=fopen(volumeNode,"w"); fputs("4\n",f); fclose(f);
    initialized=true; readVolume();
    int p[2]; assert(pipe(p)==0); fcntl(p[0],F_SETFL,O_NONBLOCK); fds[0]=p[0]; fdCount=1;
    input_event e={}; e.type=EV_KEY; e.code=KEY_VOLUMEUP; e.value=1;
    assert(write(p[1],&e,sizeof(e))==sizeof(e)); SDL_Delay(15); rgdsPollHardware(); assert(level==5);
    f=fopen(volumeNode,"r"); int saved=-1; assert(fscanf(f,"%d",&saved)==1); fclose(f); assert(saved==5);
    e.code=KEY_VOLUMEDOWN; assert(write(p[1],&e,sizeof(e))==sizeof(e)); SDL_Delay(15); rgdsPollHardware(); assert(level==4);
    close(p[0]);close(p[1]);unlink(volumeNode); SDL_Quit(); puts("PASS: quadratic gain, mute, maximum, volume keys, persisted level");
}
