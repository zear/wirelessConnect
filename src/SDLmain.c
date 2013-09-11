#include "SDLmain.h"
#include <SDL.h>
#include "global.h"

SDL_Surface *screen;
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

	SDL_ShowCursor(SDL_DISABLE);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if(screen == NULL)
	{
		fprintf(stderr, "ERROR (initSDL): Failed to set the screen surface.\n");
		return 1;
	}
        SDL_WM_SetCaption("Wireless Connect", NULL);

	return 0;
}

void cleanSDL()
{
	//SDL_FreeSurface(Logo.bitmap);
	SDL_Quit();
}
