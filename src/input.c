#include "input.h"
#include <SDL.h>
#include "logic.h"

_Bool keystate[2048];
int keyDelay[2048];

SDL_Event event;

void input()
{
	if(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				setGameState(STATE_EXIT);
				break;
			case SDL_KEYDOWN:
				keystate[event.key.keysym.sym] = 1;
				break;
			case SDL_KEYUP:
				keystate[event.key.keysym.sym] = 0;
				keyDelay[event.key.keysym.sym] = 0;
				break;
			default:
				break;
		}
	}
}

int keyDelayCheck(SDLKey Key, int newDelay)
{
	if(keystate[Key] != 0)
	{
		if(keyDelay[Key] > 0)
		{
			keyDelay[Key]--;
			return 1;
		}
		else
		{
			keyDelay[Key] = newDelay;
			return 0;
		}
	}
	else
	{
		return 1;
	}
}
