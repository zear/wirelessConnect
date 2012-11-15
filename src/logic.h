#ifndef _LOGIC_H_
#define _LOGIC_H_

enum GameStateType
{
	STATE_EXIT = 0,
	STATE_INIT,
	STATE_MENU,
};
extern enum GameStateType GameState;
extern enum GameStateType NewGameState;

void setGameState(enum GameStateType newState);
void changeGameState();
void logicInit();
void logicMenu();
void logic();
void clean();

void unloadMenu();
void unloadGame();

extern int quit;

#endif /* _LOGIC_H_ */
