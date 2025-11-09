#ifndef GAME
#define GAME


#include "types.h"

void initGame();

Room *generateRoom(Dungeon *dungeon);
Room *trapRoom(Room *area, Dungeon *dungeon);
Room *combatRoom(Room *area, Dungeon *dungeon);

void swampDungeon();
void mansionDungeon();
void caveDungeon();

#endif