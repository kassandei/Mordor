#ifndef GAME
#define GAME


#include "types.h"

void initGame();
void swampDungeon();
void mansionDungeon();
void caveDungeon();

int rollDice();
void gameOver();
void combat(Monster* monster);

#endif