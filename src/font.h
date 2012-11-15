#ifndef _FONT_H_
#define _FONT_H_

#include <SDL.h>

typedef struct FontFileStruct
{
	SDL_Surface *bitmap;
	SDL_Rect clip[256];
	int width;
	int height;

} Font;

extern Font FontSmall;
extern Font FontLarge;

int loadFont(Font *Font, char *fileName, int width, int height);

#endif /* _FONT_H_ */
