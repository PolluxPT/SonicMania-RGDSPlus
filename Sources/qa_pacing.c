#include <SDL.h>
#include <dlfcn.h>
#include <stdio.h>
void SDL_RenderPresent(SDL_Renderer *r) {
 static void (*real)(SDL_Renderer *); static unsigned n;static Uint64 start;
 if(!real)real=dlsym(RTLD_NEXT,"SDL_RenderPresent");
 real(r);++n;
 if(n==10) start=SDL_GetPerformanceCounter();
 if(n==190) {
   double seconds=(SDL_GetPerformanceCounter()-start)/(double)SDL_GetPerformanceFrequency();
   fprintf(stderr,"QA pacing: 180 intervals in %.6f seconds = %.3f fps\n",seconds,180/seconds);
   SDL_Event e={0};e.type=SDL_QUIT;SDL_PushEvent(&e);
 }
}
