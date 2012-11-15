#ifndef _SDLMAIN_H_
#define _SDLMAIN_H_

#include <SDL.h>

extern SDL_Surface *screen;
extern SDL_Joystick *joy;

int initSDL();
void cleanSDL();

#endif /* _SDLMAIN_H_ */
