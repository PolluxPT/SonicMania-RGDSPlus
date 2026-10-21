#include <SDL.h>
#include <string.h>
int SDL_NumJoysticks(void){return 1;}
const char *SDL_JoystickNameForIndex(int i){return "ANBERNIC QA";}
SDL_Joystick *SDL_JoystickOpen(int i){return (SDL_Joystick*)0x123;}
int SDL_JoystickNumAxes(SDL_Joystick *j){return 4;}
void SDL_JoystickClose(SDL_Joystick *j){}
Sint16 SDL_JoystickGetAxis(SDL_Joystick *j,int axis){
 Uint32 t=SDL_GetTicks();
 if(t>=4000&&t<5000&&axis==3)return -25000;
 if(t>=6000&&t<7000&&axis==0)return 25000;
 return 0;
}
SDL_JoystickGUID SDL_JoystickGetGUID(SDL_Joystick *j){
 SDL_JoystickGUID g={{0x19,0,0xb6,0x55,1,0,0,0,1,0,0,0,0,1,0,0}};return g;
}
