#include "logic.h"
#include "fileio.h"
#include "font.h"
#include "input.h"
#include "menu.h"
#include "SDLmain.h"
#include "network.h"
#include "onScreenKeyboard.h"

enum GameStateType GameState = STATE_INIT;
enum GameStateType NewGameState;
int quit;
Network CurNetwork;

void setGameState(enum GameStateType NewState)
{
	NewGameState = NewState;
}

void changeGameState()
{
	// unload current
	switch(GameState)
	{
		case STATE_MENU:
			break;

		default:
			break;
	}

	// load new
	switch(NewGameState)
	{
		case STATE_MENU:
			loadConfig(homeDir, "/config.cfg");
			//loadConfig("./data/config.cfg");
			break;

		default:
			break;
	}

	GameState = NewGameState;
}

void logicInit()
{
	// init all program assets that we keep through the entire run time
	loadFont(&FontSmall, "./data/fontSmall.bmp", 5, 9);
	loadFont(&FontLarge, "./data/fontLarge.bmp", 10, 18);
	loadFont(&FontUnifont, "./data/fontUnifont.bmp", 8, 15);

	strcpy(CurNetwork.interface, "wlan0");
	if(loadTemp("/tmp", "/.wiCon.tmp"))
	{
		CurNetwork.status = STATUS_OFF;
	}
	else
	  updateIpAddress();
	Keyboard.inited = 0;
	Keyboard.input = NULL;

	menuLoadAll();

	setGameState(STATE_MENU);
}

void logicMenu()
{
	int leaving = 0;

	if(Keyboard.enabled)
	{
		OSKeyboardInput();
	}
	else
	{
		menuInput();
	}

	if(leaving)
	{
		setGameState(STATE_EXIT);
	}
}

void logic()
{
	if(NewGameState != GameState)
	{
		changeGameState();
	}

	switch(GameState)
	{
		case STATE_EXIT:
			quit = 1;
			break;
		case STATE_INIT:
			logicInit();
			break;
		case STATE_MENU:
			logicMenu();
			break;

		default:
		break;
	}
}

void clean()
{
	cleanSDL();
	menuDeleteAll();
}
