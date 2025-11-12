#ifndef DUNGEON
#define DUNGEON


#include "types.h"

Room* generateRoom(Dungeon *dungeon);
Room *generateRoomSwamp(Room *room);
Room *generateRoomMansion(Room *room);
Room *generateRoomCave(Room *room);
Room *trapRoom(Room *area, DungeonType type, int random);
Room* combatRoom(Room *area, DungeonType type, int random);

#endif