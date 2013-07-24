#ifndef _ON_SCREEN_KEYBOARD_H_
#define _ON_SCREEN_KEYBOARD_H_

#define VISIBILITY_LEN 2400

typedef enum OnScrKbdTypeEnum
{
	OSK_ALPHANUM,
	OSK_NUMERIC,
	OSK_IP
} OnScrKbdType;

typedef struct OnScrKbdStruct
{
	OnScrKbdType type;
	int inited;
	int enabled;
	char *input;
	char *source;
	int inputLen;
	int inputPos;
	int cursorPx;
	int cursorPy;
	int cursorPnum;
	int visTimer;
} OnScrKbd;

extern OnScrKbd Keyboard;
void OSKeyboardInput();

#endif /* _ON_SCREEN_KEYBOARD_H_ */
