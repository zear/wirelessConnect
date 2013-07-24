#include "SDLgfx.h"
#include <SDL.h>
#include <stdlib.h>
#include "config.h"
#include "font.h"
#include "onScreenKeyboard.h"
#include "SDLmain.h"

SDL_Surface *loadImage(char *fileName)
{
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;
	
	if(fileName == NULL)
	{
		fprintf(stderr, "ERROR (loadImage): File name not specified!\n");
		return NULL;
	}
	
	loadedImage = SDL_LoadBMP(fileName);
	
	if(loadedImage == NULL)
	{
		fprintf(stderr, "ERROR (loadImage): Failed to load image file: %s\n", fileName);
		return NULL;
	}
	else
	{
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	
	if(optimizedImage == NULL)
	{
		fprintf(stderr, "ERROR (loadImage): Failed to optimize image file: %s\n", fileName);
		return NULL;
	}
	else
	{
		// set transparency to magenta
		Uint32 colorKey = SDL_MapRGB(optimizedImage->format, 255, 0, 255);
		SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorKey);
	}
	
	return optimizedImage;
}

void drawText(char *text, int x, int y, Font *Font)
{
	int c; // current character
	int posX = x;
	int posY = y;

	for(c = 0; text[c] != '\0'; c++)
	{
		if(text[c] == '\n')
		{
			y++;
			posY += Font->height;
			posX = x;
		}
		else
		{
			applySurface(Font->bitmap, screen, posX, posY, &Font->clip[(int)text[c]]);
			posX += Font->width;
		}
	}
}

void drawTextConcat(char *label, char *value, int x, int y, Font *font){
  char buff[128];

  snprintf(buff, 128, "%s%s", label, value);
  drawText(buff, x, y, font);
}

void drawBackground(int r, int g, int b)
{
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, r, g, b));
}

void drawKeyboard()
{
	Font *CurFont = &FontUnifont;
	char character[4];
	char atText[10];
	char lenText[10];
	int posX = 0;
	int posY = 0;
	char i;

	switch(Keyboard.kbdType)
	{
		case OSK_ALPHANUM:
			for(i = 32; i < 126; i++, posX++)
			{
				if(posX > 15)
				{
					posX = 0;
					posY++;
				}

				if(Keyboard.cursorPx == posX && Keyboard.cursorPy == posY)
				{
					character[0] = '[';
					character[1] = i;
					character[2] = ']';
					character[3] = '\0';
				}
				else
				{
					character[0] = ' ';
					character[1] = i;
					character[2] = ' ';
					character[3] = '\0';
				}

				if(Keyboard.textType == OSK_TEXT_VISIBLE)
				{
					drawText(Keyboard.input, 20, 20, CurFont);
				}
				else if(Keyboard.textType == OSK_TEXT_HIDDEN)
					{
					if(config.showKey)
					{
						drawText(Keyboard.input, 20, 20, CurFont);
					}
					else
					{
						int i;

						for(i = 0; i < strlen(Keyboard.input) - 1; i++)
						{
							drawText("*", 20 + i * CurFont->width, 20, CurFont);
						}

						if(Keyboard.visTimer > 0)
						{
							drawText(&Keyboard.input[i], 20 + i * CurFont->width, 20, CurFont);
							Keyboard.visTimer--;
						}
						else
						{
							drawText("*", 20 + i * CurFont->width, 20, CurFont);
						}
					}
				}
				drawText("^", 20 + Keyboard.inputPos * CurFont->width, 35, CurFont);
				sprintf(atText, "At:  %d", Keyboard.inputPos);
				drawText(atText, 250, 5, CurFont);
				sprintf(lenText, "Max: %d", Keyboard.inputLen);
				drawText(lenText, 250, 20, CurFont);
				drawText(character, (posX * CurFont->width * 2) + 20, (posY * CurFont->height) + 50, CurFont);

				if(Keyboard.cursorPx > 13 && Keyboard.cursorPy == 5)
				{
					drawText("*OK*", 14 * CurFont->width * 2 + 25, (5 * CurFont->height) + 50, CurFont);
				}
				else
				{
					drawText(" OK ", 14 * CurFont->width * 2 + 25, (5 * CurFont->height) + 50, CurFont);
				}
			}
			break;
		case OSK_NUMERIC:
			for(i = 48; i < 58; i++, posX++)
			{
				if(posX > 15)
				{
					posX = 0;
					posY++;
				}

				if(Keyboard.cursorPx == posX && Keyboard.cursorPy == posY)
				{
					character[0] = '[';
					character[1] = i;
					character[2] = ']';
					character[3] = '\0';
				}
				else
				{
					character[0] = ' ';
					character[1] = i;
					character[2] = ' ';
					character[3] = '\0';
				}

				drawText(Keyboard.input, 20, 20, CurFont);
				drawText("^", 20 + Keyboard.inputPos * CurFont->width, 35, CurFont);
				sprintf(atText, "At:  %d", Keyboard.inputPos);
				drawText(atText, 250, 5, CurFont);
				sprintf(lenText, "Max: %d", Keyboard.inputLen);
				drawText(lenText, 250, 20, CurFont);
				drawText(character, (posX * CurFont->width * 2) + 20, (posY * CurFont->height) + 50, CurFont);

				if(Keyboard.cursorPy > 0)
				{
					drawText("*OK*", 14 * CurFont->width * 2 + 25, (5 * CurFont->height) + 50, CurFont);
				}
				else
				{
					drawText(" OK ", 14 * CurFont->width * 2 + 25, (5 * CurFont->height) + 50, CurFont);
				}
			}
			break;
		case OSK_IP:
			for(i = 48; i < 59; i++, posX++)
			{
				if(posX > 15)
				{
					posX = 0;
					posY++;
				}

				if(Keyboard.cursorPx == posX && Keyboard.cursorPy == posY)
				{
					character[0] = '[';
					character[1] = i;
					character[2] = ']';
					character[3] = '\0';
				}
				else
				{
					character[0] = ' ';
					character[1] = i;
					character[2] = ' ';
					character[3] = '\0';
				}

				if(character[1] == 58)
				{
					character[1] = '.';
				}

				drawText(Keyboard.input, 20, 20, CurFont);
				drawText("^", 20 + Keyboard.inputPos * CurFont->width, 35, CurFont);
				sprintf(atText, "At:  %d", Keyboard.inputPos);
				drawText(atText, 250, 5, CurFont);
				sprintf(lenText, "Max: %d", Keyboard.inputLen);
				drawText(lenText, 250, 20, CurFont);
				drawText(character, (posX * CurFont->width * 2) + 20, (posY * CurFont->height) + 50, CurFont);

				if(Keyboard.cursorPy > 0)
				{
					drawText("*OK*", 14 * CurFont->width * 2 + 25, (5 * CurFont->height) + 50, CurFont);
				}
				else
				{
					drawText(" OK ", 14 * CurFont->width * 2 + 25, (5 * CurFont->height) + 50, CurFont);
				}
			}
			break;

		default:
			break;
	}
}

void clipImage(SDL_Rect *source, int tileWidth, int tileHeight, int rows, int numOfTiles)
{
	int i = 0; // current tile pos y
	int j = 0; // current tile pos x
	int k = 0; // current tile number
	int l = 0; // current tile row

	for(i = 0; k < numOfTiles; i+= tileHeight)
	{
		for(j = 0; l < rows; j+= tileWidth)
		{
			source[k].x = j;
			source[k].y = i;
			source[k].w = tileWidth;
			source[k].h = tileHeight;
			++k;
			++l;
		}

		l = 0;
	}
}

void applySurface(SDL_Surface *source, SDL_Surface *destination, int x, int y, SDL_Rect *clip)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

int updateScreen()
{
	if(SDL_Flip(screen) == -1)
	{
		fprintf(stderr, "ERROR (updateScreen): Failed to flip screen surface!\n");
		return 1;
	}

	return 0;
}
