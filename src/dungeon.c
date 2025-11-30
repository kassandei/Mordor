#include <stdlib.h>
#include "game.h"
#include "dungeon.h"

Monster swampMonsters[SWAMP_MONSTERS] = {
    {"Cane Selvaggio", 2, 1, 0},      // fatalBlow, dmg, coin
    {"Goblin", 3, 2, 2},
    {"Scheletro", 4, 2, 4},
    {"Orco", 3, 4, 6},
    {"Generale Orco", 6, 3, 12}
};

static Trap swampTrap = {
    .name = "Acquitrino Velenoso",
};

Room* generateRoom(Dungeon* dungeon) {
    Room *area = (Room*)malloc(sizeof(Room));
    switch (dungeon->mission) {
        case SWAMP:
            area = generateRoomSwamp(area);
            break;
        case MANSION:
            area = generateRoomMansion(area);
            break;
        case CAVE:
            area = generateRoomCave(area);
            break;
        default:
            break;
    }
    return area;
}

Room* generateRoomSwamp(Room* area) {
    int roomType = rand() % 2 + 1;
    int randomMonster = rand() % SWAMP_MONSTERS;
    area->type = roomType;
    if(area->type == TRAP) {
        area = trapRoom(area, SWAMP, rand() % 5 + 1);
    }
    else {
        area = combatRoom(area, SWAMP, randomMonster);
    }
    return area;
}
Room* generateRoomMansion(Room* area) {

}
Room* generateRoomCave(Room* area) {

}


Room* trapRoom(Room* area, DungeonType type, int random) {
    if(type == SWAMP) {
        area->trap.name = swampTrap.name;
        area->trap.dmg = random;
    }
    return area;
}

Room* combatRoom(Room* area, DungeonType type, int random) {
    if(type == SWAMP) {
        area->monster = swampMonsters[random];
    }

    return area;
}