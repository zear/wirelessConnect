#include "SDLmain.h"
#include <SDL.h>
#include "global.h"

SDL_Surface *screen = NULL;
SDL_Joystick *joy;

int initSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK))
	{
		fprintf(stderr, "ERROR (initSDL): Failed to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_JoystickEventState(SDL_ENABLE);

	joy = SDL_JoystickOpen(0);

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	SDL_ShowCursor(SDL_DISABLE);
        SDL_WM_SetCaption("Wireless Connect 0.1", NULL);

	return 0;
}

void cleanSDL()
{
	//SDL_FreeSurface(Logo.bitmap);
	SDL_Quit();
}
