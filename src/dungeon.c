#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "dungeon.h"
#include "types.h"

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

Monster mansionMonsters[MANSION_MONSTERS] = {
    {"Pipistrello", 2, 2, 1},
    {"Zombie", 3, 2, 2},
    {"Fanstasma", 5, 2, 4},
    {"Vampiro Superiore", 4, 4, 7},
    {"Demone Custode", 4, 6, 10}
};

static Trap mansionTrap = {
    .name = "Botola Buia",
    .dmg = 3
};


Dungeon* generateDungeon(Dungeon* dungeon) {
    // crea il primo nodo
    Room* start = generateRoom(dungeon);
    start->roomNumber = 0;

    Room* current = start;

    for (int i = 1; i < DUNGEON_ROOMS; i++) {
        Room* next = generateRoom(dungeon);
        next->roomNumber = i;
        current->nextRoom = next;
        current = next;
    }

    current->nextRoom = NULL;
    dungeon->room = start;
    return dungeon;
}


Room* generateRoom(Dungeon* dungeon) {
    Room *area = (Room*)malloc(sizeof(Room));
    if (!area) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
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
    int roomType = rand() % 2 + 1;
    int randomMonster = rand() % MANSION_MONSTERS;
    area->type = roomType;
    if(area->type == TRAP) {
       area = trapRoom(area, MANSION, mansionTrap.dmg);
    }
    else {
        area = combatRoom(area, MANSION, randomMonster);
    }
    return area;
}
Room* generateRoomCave(Room* area) {
    // TODO: Implementare generazione stanze per la grotta
    area->type = COMBAT;
    return area;
}


Room* trapRoom(Room* area, DungeonType type, int random) {
    if(type == SWAMP) {
        area->trap.name = swampTrap.name;
        area->trap.dmg = random;
    }
    if(type == MANSION) {
        area->trap.name = mansionTrap.name;
        area->trap.dmg = random;
    }

    return area;
}

Room* combatRoom(Room* area, DungeonType type, int random) {
    if(type == SWAMP) {
        area->monster = swampMonsters[random];
    }
    if(type == MANSION) {
        area->monster = mansionMonsters[random];
    }

    return area;
}


void freeDungeon(Dungeon* dungeon) {
    Room* current = dungeon->room;
    while(current != NULL) {
        Room* tmp = current->nextRoom;
        free(current);
        current = tmp;
    }
    free(dungeon);
}