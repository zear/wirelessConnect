#include "draw.h"
#include "font.h"
#include "global.h"
#include "input.h"
#include "logic.h"
#include "menu.h"
#include "network.h"
#include "onScreenKeyboard.h"
#include "SDLgfx.h"
#include "SDLmain.h"

char *mode[] =
    {
        "MANAGED",
        "AD-HOC",
        "MASTER"
    };

char *encode[] =
    {
        "NONE (OPEN)",
        "WEP",
        "WEP (NUMERIC KEY)",
        "WPA"
    };

char *status[] =
    {
        "OFF",
        "ON",
        "CONNECTING...",
        "FAILED TO CONNECT"
    };

void drawMenu()
{
	drawBackground(0, 0, 0);

	if(Keyboard.enabled)
	{
		if(Keyboard.input != NULL) // TODO: We should instead only call this part in the next iteration of the loop
		{
			drawKeyboard();
		}

		drawText("A: TYPE", 15, 200, &FontUnifont);
		drawText("B: DELETE", 90, 200, &FontUnifont);
		drawText("R: CLEAR", 200, 200, &FontUnifont);
		drawText("Y: SPACE", 15, 215, &FontUnifont);
		drawText("START: APPLY", 90, 215, &FontUnifont);
		drawText("SELECT: REJECT", 200, 215, &FontUnifont);
	}
	else
	{
		drawTextConcat("CARD:   ", CurNetwork.interface, 70, 5, &FontUnifont);
		drawTextConcat("MODE:   ", mode[CurNetwork.mode], 70, 20, &FontUnifont);
		drawTextConcat("ESSID:  ", CurNetwork.essid, 70, 35, &FontUnifont);
		drawTextConcat("ENC:    ", encode[CurNetwork.encryption], 70, 50, &FontUnifont);
		drawTextConcat("PASS:   ", (CurNetwork.encryption == ENC_NONE) ? "N/A": CurNetwork.key, 70, 65, &FontUnifont);
		drawTextConcat("STATUS: ", status[CurNetwork.status], 70, 80, &FontUnifont);

		drawText("IP:", 70, 95, &FontUnifont);

		drawText("v0.1.0", SCREEN_WIDTH - 30, SCREEN_HEIGHT - FontSmall.height, &FontSmall);

		menuDraw(CurrentMenu, 100, 115);
	}
}

void draw()
{
	switch(GameState)
	{
		case STATE_MENU:
			drawMenu();
			break;

		default:
			break;
	}

	updateScreen();
}
