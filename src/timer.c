#include "timer.h"
#include <math.h>
#include <SDL.h>
#include "global.h"

int frameRate = 0;
int deltaTime = 1;
int oDeltaTime= 1;
float frameScale = 0.f;
static int currentTime = 0; //SDL_GetTicks();

void doFrame() {
	int newTime;
	
	oDeltaTime = deltaTime;
	newTime = SDL_GetTicks();
	deltaTime = newTime - currentTime;
	deltaTime *= oDeltaTime;
	
	deltaTime = sqrt(deltaTime);
	
	currentTime = newTime;
	
	frameScale = (float)deltaTime/1000;
	
#if defined(NO_FPS_LIMIT)
	SDL_Delay(1); //Don't go faster than the speed of light, please.
#else
	if (deltaTime < FPS)
		SDL_Delay(FPS - deltaTime);
#endif
}

int calculateFrameRate()
{
	static double accumulator = 0;
	static double frameTime = 1000.0/(double)FPS;
	int newTime;
	int sleep = 1;

	newTime = SDL_GetTicks();
	deltaTime = newTime - currentTime;
	currentTime = newTime;
#if defined(NO_FPS_LIMIT)
	accumulator = frameTime;
#else
	accumulator += deltaTime;
#endif

	while(accumulator >= frameTime)
	{
		frameRate = 1000/accumulator; // NOTE: This counts how many frames at the current frame draw time would be possible to draw in 1000ms (1s). It is not actual FPS.
#if !defined(NO_FPS_LIMIT)
		if(frameRate > 60)
		{
			frameRate = 60;
		}
#endif
		accumulator -= frameTime;
		sleep = 0;
	}

	frameScale = (float)deltaTime/1000;
	
	return sleep;
}
