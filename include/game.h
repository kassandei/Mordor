#ifndef GAME
#define GAME


#include "types.h"

void initGame();
void initGameSaves();
void freeGameSaves();

void swampDungeon();
void mansionDungeon();
void caveDungeon();
void bossFight();

void gameOver();
void combat(Monster* monster);
void finalcombat(Move playerMove, BossRoom* room);

#endif