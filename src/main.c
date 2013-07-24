#include "config.h"
#include "draw.h"
#include "fileio.h"
#include "input.h"
#include "logic.h"
#include "SDLmain.h"
#include "timer.h"

Config config;

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
	saveNetworkConfig(homeDir, "/lastNetwork.cfg");

#ifdef _NO_IFADDRS
	saveTemp("/tmp", "/.wiCon.tmp");
#endif

	clean();

	return 0;
}
