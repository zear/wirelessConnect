#include "draw.h"
#include "font.h"
#include "input.h"
#include "logic.h"
#include "menu.h"
#include "network.h"
#include "onScreenKeyboard.h"
#include "SDLgfx.h"
#include "SDLmain.h"

void drawMenu()
{
	drawBackground(0, 0, 0);

	if(Keyboard.enabled)
	{
		if(Keyboard.input != NULL) // TODO: We should instead only call this part in the next iteration of the loop
		{
			drawKeyboard();
		}

		drawText("A: TYPE", 70, 200, &FontSmall);
		drawText("B: DELETE", 140, 200, &FontSmall);
		drawText("R: CLEAR", 210, 200, &FontSmall);
		drawText("Y: SPACE", 70, 210, &FontSmall);
		drawText("START: APPLY", 140, 210, &FontSmall);
		drawText("SELECT: REJECT", 210, 210, &FontSmall);
	}
	else
	{
		//drawText("Wireless Connect", 100, 20, &FontLarge);

		drawText("MODE:", 70, 40, &FontSmall);
		switch(CurNetwork.mode)
		{
			case NMODE_ADHOC:
				drawText("AD-HOC", 110, 40, &FontSmall);
			break;
			case NMODE_MANAGED:
				drawText("MANAGED", 110, 40, &FontSmall);
			break;
			case NMODE_MASTER:
				drawText("MASTER", 110, 40, &FontSmall);
			break;

			default:
			break;
		}
		drawText("ESSID:", 70, 50, &FontSmall);
		drawText(CurNetwork.essid, 110, 50, &FontSmall);
		drawText("ENC:", 70, 60, &FontSmall);
		switch(CurNetwork.encryption)
		{
			case ENC_NONE:
				drawText("NONE (OPEN)", 110, 60, &FontSmall);
			drawText("PASS:", 70, 70, &FontSmall);
			drawText("N/A", 110, 70, &FontSmall);
			break;
			case ENC_WEP:
				drawText("WEP", 110, 60, &FontSmall);
			drawText("PASS:", 70, 70, &FontSmall);
			drawText(CurNetwork.key, 110, 70, &FontSmall);

			break;
			case ENC_WPA:
				drawText("WPA", 110, 60, &FontSmall);
			drawText("PASS:", 70, 70, &FontSmall);
			drawText(CurNetwork.key, 110, 70, &FontSmall);

			break;

			default:
			break;
		}
		drawText("STATUS:", 70, 80, &FontSmall);
		switch(CurNetwork.status)
		{
			case STATUS_ON:
			drawText("ON", 110, 80, &FontSmall);
			break;
			case STATUS_OFF:
			drawText("OFF", 110, 80, &FontSmall);
			break;

			default:
			break;
		}
		//drawText(CurNetwork.status, 110, 80, &FontSmall);

		menuDraw(CurrentMenu, 120, 100);
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
