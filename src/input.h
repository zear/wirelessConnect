#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL.h>

extern _Bool keystate[2048];
extern int keyDelay[2048];

void input();
int keyDelayCheck(SDLKey Key, int newDelay);

#endif /* _INPUT_H_ */
