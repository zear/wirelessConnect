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

		drawText("A: TYPE", 15, 200, &FontUnifont);
		drawText("B: DELETE", 90, 200, &FontUnifont);
		drawText("R: CLEAR", 200, 200, &FontUnifont);
		drawText("Y: SPACE", 15, 215, &FontUnifont);
		drawText("START: APPLY", 90, 215, &FontUnifont);
		drawText("SELECT: REJECT", 200, 215, &FontUnifont);
	}
	else
	{
		//drawText("Wireless Connect", 100, 20, &FontLarge);

		drawText("MODE:", 70, 20, &FontUnifont);
		switch(CurNetwork.mode)
		{
			case NMODE_ADHOC:
				drawText("AD-HOC", 130, 20, &FontUnifont);
			break;
			case NMODE_MANAGED:
				drawText("MANAGED", 130, 20, &FontUnifont);
			break;
			case NMODE_MASTER:
				drawText("MASTER", 130, 20, &FontUnifont);
			break;

			default:
			break;
		}
		drawText("ESSID:", 70, 35, &FontUnifont);
		drawText(CurNetwork.essid, 130, 35, &FontUnifont);
		drawText("ENC:", 70, 50, &FontUnifont);
		switch(CurNetwork.encryption)
		{
			case ENC_NONE:
			drawText("NONE (OPEN)", 130, 50, &FontUnifont);
			drawText("PASS:", 70, 65, &FontUnifont);
			drawText("N/A", 130, 65, &FontUnifont);
			break;
			case ENC_WEP:
			drawText("WEP", 130, 50, &FontUnifont);
			drawText("PASS:", 70, 65, &FontUnifont);
			drawText(CurNetwork.key, 130, 65, &FontUnifont);

			break;
			case ENC_WPA:
			drawText("WPA", 130, 50, &FontUnifont);
			drawText("PASS:", 70, 65, &FontUnifont);
			drawText(CurNetwork.key, 130, 65, &FontUnifont);

			break;

			default:
			break;
		}
		drawText("STATUS:", 70, 80, &FontUnifont);
		switch(CurNetwork.status)
		{
			case STATUS_ON:
			drawText("ON", 130, 80, &FontUnifont);
			break;
			case STATUS_OFF:
			drawText("OFF", 130, 80, &FontUnifont);
			break;
			case STATUS_CONNECTING:
			drawText("CONNECTING...", 130, 80, &FontUnifont);
			break;
			case STATUS_FAILED:
			drawText("FAILED TO CONNECT", 130, 80, &FontUnifont);
			break;

			default:
			break;
		}
		//drawText(CurNetwork.status, 130, 80, &FontSmall);

		menuDraw(CurrentMenu, 100, 100);
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
