#ifndef _ON_SCREEN_KEYBOARD_H_
#define _ON_SCREEN_KEYBOARD_H_

typedef struct OnScrKbdStruct
{
	int inited;
	int enabled;
	char *input;
	char *source;
	int inputLen;
	int inputPos;
	int cursorPx;
	int cursorPy;
	int cursorPnum;
} OnScrKbd;

extern OnScrKbd Keyboard;

void OSKeyboardInput();

#endif /* _ON_SCREEN_KEYBOARD_H_ */
