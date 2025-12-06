#ifndef GAME
#define GAME


#include "types.h"

void initGame();
void swampDungeon();
void mansionDungeon();
void caveDungeon();

void gameOver();
void combat(Monster* monster);

#endif