#include <SDL.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
static unsigned captureFrame;
Uint8 SDL_GameControllerGetButton(SDL_GameController *controller, SDL_GameControllerButton button) {
    static Uint8 (*real)(SDL_GameController *, SDL_GameControllerButton);
    if (!real) real=dlsym(RTLD_NEXT,"SDL_GameControllerGetButton");
    if (getenv("QA_QUIT_COMBO") && captureFrame >= 30
        && (button == SDL_CONTROLLER_BUTTON_BACK || button == SDL_CONTROLLER_BUTTON_START)) return 1;
    return real(controller, button);
}
void SDL_RenderPresent(SDL_Renderer *r) {
    static unsigned frame;
    static SDL_Joystick *pad;
    static void (*present)(SDL_Renderer *);
    if (!present) present = dlsym(RTLD_NEXT, "SDL_RenderPresent");
    present(r);
    ++frame;
    captureFrame=frame;
    if (frame == 1) {
        int index = SDL_JoystickAttachVirtual(SDL_JOYSTICK_TYPE_GAMECONTROLLER, 2, 15, 1);
        fprintf(stderr, "QA virtual index=%d error=%s\n", index, SDL_GetError());
        if (index >= 0) {
            char guid[64], mapping[512];
            SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(index), guid, sizeof(guid));
            snprintf(mapping, sizeof(mapping), "%s,QA Controller,a:b0,b:b1,x:b2,y:b3,back:b4,start:b6,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,leftx:a0,lefty:a1,platform:Linux,", guid);
            int mapped = SDL_GameControllerAddMapping(mapping);
            pad = SDL_JoystickOpen(index);
            fprintf(stderr, "QA mapping=%d recognized=%d mappingerr=%s\n", mapped, SDL_IsGameController(index), SDL_GetError());
            SDL_Event add = {0}; add.type = SDL_CONTROLLERDEVICEADDED; add.cdevice.which = index; SDL_PushEvent(&add);
        }
    }
    unsigned limit = getenv("QA_FRAMES") ? atoi(getenv("QA_FRAMES")) : 2460;
    if (pad && getenv("QA_QUIT_COMBO") && frame == 30) {
        int a=SDL_JoystickSetVirtualButton(pad, 4, 1);
        int b=SDL_JoystickSetVirtualButton(pad, 6, 1);
        SDL_JoystickUpdate();
        fprintf(stderr,"QA setcombo %d/%d err=%s\n",a,b,SDL_GetError());
    }
    if (pad && getenv("QA_QUIT_COMBO") && frame == 60) {
        SDL_GameController *c = SDL_GameControllerFromInstanceID(SDL_JoystickInstanceID(pad));
        fprintf(stderr,"QA combo raw=%d/%d mapped=%d/%d\n",SDL_JoystickGetButton(pad,4),SDL_JoystickGetButton(pad,6),c?SDL_GameControllerGetButton(c,SDL_CONTROLLER_BUTTON_BACK):-1,c?SDL_GameControllerGetButton(c,SDL_CONTROLLER_BUTTON_START):-1);
    }
    if (frame == 180 || frame == 420 || frame == 960 || frame == 1500 || frame == 2400 || frame + 1 == limit) {
        int w, h; SDL_GetRendererOutputSize(r, &w, &h);
        SDL_Surface *s = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);
        if (s && SDL_RenderReadPixels(r, NULL, s->format->format, s->pixels, s->pitch) == 0) {
            char path[64]; snprintf(path, sizeof(path), "capture-%u.bmp", frame); SDL_SaveBMP(s, path);
        }
        SDL_FreeSurface(s);
    }
    if (frame >= 720 && (frame % 120 == 0 || frame % 120 == 20)) {
        SDL_Event e = {0};
        e.type = frame % 120 == 0 ? SDL_KEYDOWN : SDL_KEYUP;
        e.key.state = e.type == SDL_KEYDOWN ? SDL_PRESSED : SDL_RELEASED;
        e.key.keysym.scancode = frame < 1000 ? SDL_SCANCODE_RETURN : SDL_SCANCODE_A;
        e.key.keysym.sym = frame < 1000 ? SDLK_RETURN : SDLK_a;
        SDL_PushEvent(&e);
        if (pad) {
            SDL_JoystickSetVirtualButton(pad, frame < 1000 ? 6 : 0, frame % 120 == 0 ? 1 : 0);
        }
    }
    if (frame == limit) { SDL_Event e = {0}; e.type = SDL_QUIT; SDL_PushEvent(&e); }
}
