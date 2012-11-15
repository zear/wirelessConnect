#include "onScreenKeyboard.h"
#include <SDL.h>
#include "input.h"

OnScrKbd Keyboard;

void OSKeyboardFetchCursor(int x, int y)
{
	
}

void OSKeyboardInput()
{
	if(!Keyboard.inited)
	{
		Keyboard.inputLen = 60;
		//Keyboard.inputLen = strlen(Keyboard.source) + 1;
		Keyboard.input = calloc(Keyboard.inputLen, 1);
		//Keyboard.input = malloc(Keyboard.inputLen);
		strcpy(Keyboard.input, Keyboard.source);
		Keyboard.inputPos = strlen(Keyboard.source);
		Keyboard.inited = 1;
	}

	if(keystate[SDLK_UP])
	{
		keystate[SDLK_UP] = 0;
		Keyboard.cursorPy--;
	}
	if(keystate[SDLK_DOWN])
	{
		keystate[SDLK_DOWN] = 0;
		Keyboard.cursorPy++;
	}
	if(keystate[SDLK_LEFT])
	{
		keystate[SDLK_LEFT] = 0;
		Keyboard.cursorPx--;
	}
	if(keystate[SDLK_RIGHT])
	{
		keystate[SDLK_RIGHT] = 0;
		Keyboard.cursorPx++;
	}

	if(Keyboard.cursorPx < 0)
	{
		Keyboard.cursorPx = 15;
	}
	else if(Keyboard.cursorPx > 15)
	{
		Keyboard.cursorPx = 0;
	}
	if(Keyboard.cursorPy > 5)
	{
		Keyboard.cursorPy = 0;
	}
	else if(Keyboard.cursorPy < 0)
	{
		Keyboard.cursorPy = 5;
	}

	if(Keyboard.inputPos < 0)
	{
		Keyboard.inputPos = 0;
	}


	Keyboard.cursorPnum = (16 * (Keyboard.cursorPy + 1) - (16 - (Keyboard.cursorPx + 1))) - 1 + 32;

	if(keystate[SDLK_LCTRL])
	{
		keystate[SDLK_LCTRL] = 0;

		if(Keyboard.cursorPnum > 125) // works as ACCEPT
		{
			strcpy(Keyboard.source, Keyboard.input);
			free(Keyboard.input);
			Keyboard.input = NULL;
			Keyboard.inited = 0;
			Keyboard.enabled = 0;
		}
		else
		{
			Keyboard.input[Keyboard.inputPos] = (char)Keyboard.cursorPnum;
			Keyboard.inputPos++;
		}

	}
	if(keystate[SDLK_LALT])
	{
		keystate[SDLK_LALT] = 0;
		Keyboard.input[Keyboard.inputPos] = '\0';
		Keyboard.inputPos--;
	}
	if(keystate[SDLK_SPACE])
	{
		keystate[SDLK_SPACE] = 0;
		Keyboard.input[Keyboard.inputPos] = ' ';
		Keyboard.inputPos++;
	}
	if(keystate[SDLK_BACKSPACE])
	{
		keystate[SDLK_BACKSPACE] = 0;
		free(Keyboard.input);
		Keyboard.input = NULL;
		Keyboard.input = calloc(Keyboard.inputLen, 1);
		Keyboard.inputPos = 0;
	}
	if(keystate[SDLK_RETURN])
	{
		keystate[SDLK_RETURN] = 0;
		strcpy(Keyboard.source, Keyboard.input);
		free(Keyboard.input);
		Keyboard.input = NULL;
		Keyboard.inited = 0;
		Keyboard.enabled = 0;
	}
	if(keystate[SDLK_ESCAPE])
	{
		keystate[SDLK_ESCAPE] = 0;
		free(Keyboard.input);
		Keyboard.input = NULL;
		Keyboard.inited = 0;
		Keyboard.enabled = 0;
	}
}
