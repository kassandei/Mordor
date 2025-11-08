#ifndef GAME
#define GAME


#include "types.h"

void initGame();

Room *generateRoom(Dungeon *dungeon);

void swampDungeon();
Room *generateSwampRoom(Dungeon *dungeon);

void mansionDungeon();
void caveDungeon();

#endif