#ifndef GAME
#define GAME


#include "types.h"

void initGame(Player *player);
void initGameSaves(GameSaves *saves);
void freeGameSaves(GameSaves *saves);

void swampDungeon(Player *player);
void mansionDungeon(Player *player);
void caveDungeon(Player *player);
void bossFight(Player *player);

void gameOver(Player *player);
void combat(Player *player, Monster* monster);
void finalcombat(Player *player, Move playerMove, BossRoom* room);

#endif