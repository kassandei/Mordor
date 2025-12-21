#ifndef DUNGEON
#define DUNGEON


#include "types.h"

#define DUNGEON_ROOMS 10

#define SWAMP_ORC 3
#define SWAMP_MONSTERS 5
#define MANSION_MONSTERS 5
extern Monster swampMonsters[SWAMP_MONSTERS];
extern Monster mansionMonsters[MANSION_MONSTERS];

Dungeon* generateDungeon(Dungeon* dungeon);
Room* generateRoom(Dungeon *dungeon);
Room *generateRoomSwamp(Room *room);
Room *generateRoomMansion(Room *room);
Room *generateRoomCave(Room *room);
Room *trapRoom(Room *area, DungeonType type, int random);
Room* combatRoom(Room *area, DungeonType type, int random);
void freeDungeon(Dungeon* dungeon);

#endif
