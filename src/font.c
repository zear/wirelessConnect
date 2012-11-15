#include "font.h"
#include <SDL.h>
#include "SDLgfx.h"

Font FontSmall;
Font FontLarge;

int loadFont(Font *Font, char *fileName, int width, int height)
{
	Font->bitmap = loadImage(fileName);

	if(Font->bitmap == NULL)
	{
		fprintf(stderr, "ERROR (loadFont): Failed to load font file: %s\n", fileName);
		return 1;
	}

	Font->width = width;
	Font->height = height;

	clipImage(Font->clip, Font->width, Font->height, 16, 256);

	return 0;
}
