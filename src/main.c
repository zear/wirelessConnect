#include "draw.h"
#include "fileio.h"
#include "input.h"
#include "logic.h"
#include "SDLmain.h"
#include "timer.h"

int main()
{
	quit = 0;

	homeDir = getHomeDir(homeDir);

	initSDL();

	setGameState(STATE_INIT);

	while(!quit)
	{
		doFrame();
		{
			input();
			logic();
			draw();
		}
	}

	saveConfig(homeDir, "/config.cfg");

	clean();

	return 0;
}
