#include <SDL.h>
#include <dlfcn.h>
#include <stdio.h>
int rand(void) { return 1234567; }
void SDL_RenderPresent(SDL_Renderer *r) {
 static void (*real)(SDL_Renderer *);static unsigned frame;
 if(!real)real=dlsym(RTLD_NEXT,"SDL_RenderPresent");
 real(r);++frame;
 if(frame==120||frame==600||frame==1200){
  int w,h;SDL_GetRendererOutputSize(r,&w,&h);
  SDL_Surface *s=SDL_CreateRGBSurfaceWithFormat(0,w,h,32,SDL_PIXELFORMAT_ARGB8888);
  if(s&&SDL_RenderReadPixels(r,NULL,s->format->format,s->pixels,s->pitch)==0){char p[64];snprintf(p,sizeof(p),"frame-%u.bmp",frame);SDL_SaveBMP(s,p);}
  SDL_FreeSurface(s);
 }
}
