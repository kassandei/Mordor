#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
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

Monster caveMonster = {
    .name = "Drago Antico",
    .fatalBlow = 5,
    .dmg = 10,
    .coin = 12
};

static Trap caveTraps[CAVE_TRAPS] = {
    {"Cristalli cadenti", 2, 0},
    {"Ponte pericolante", 0, -3},
    {"Forziere Misteriose", 2, 10},
    {"Rupe scoscesa", 6, 0}
};

Dungeon* generateDungeon(Dungeon* dungeon) {
    dungeon->start = NULL;
    dungeon->end = NULL;
    dungeon->roomCount = 0;
    return dungeon;
}

Room* addRoom(Dungeon* dungeon) {
    Room* newRoom = generateRoom(dungeon);
    newRoom->roomNumber = dungeon->roomCount;
    newRoom->nextRoom = NULL;

    if(dungeon->start == NULL) {
        dungeon->start = newRoom;
        dungeon->end = newRoom;
    }
    else {
        dungeon->end->nextRoom = newRoom;
        dungeon->end = newRoom;
    }

    dungeon->roomCount++;
    return newRoom;
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
    int roomType = rand() % 3; // considera anche la stanza vuota
    int randomTrap = rand() % CAVE_TRAPS;
    area->type = roomType;
    if(area->type == TRAP) {
        area = trapRoom(area, CAVE, randomTrap);
    }
    else {
        area = combatRoom(area, CAVE, -1);
    }
    return area;
}

Room* trapRoom(Room* area, DungeonType type, int random) {
    if(type == SWAMP) {
        area->trap.name = swampTrap.name;
        area->trap.dmg = random;
    }
    else if(type == MANSION) {
        area->trap.name = mansionTrap.name;
        area->trap.dmg = random;
    }
    else if(type == CAVE) {
        area->trap.name = caveTraps[random].name;
        if(strcmp("Forziere Misterioso", area->trap.name) == 0) {
            CoinFace toss = flipCoin();
            if(toss == HEAD) {
                area->trap.dmg = 0;
            }
            else area->trap.coin = 0;
        }
        else if(strcmp("Rupe scoscesa", area->trap.name) == 0) {
            area->trap.dmg = rollDice();
        }
        else 
            area->trap.dmg = caveTraps[random].dmg;
    }

    return area;
}

Room* combatRoom(Room* area, DungeonType type, int random) {
    if(type == SWAMP) 
        area->monster = swampMonsters[random];
    else if(type == MANSION)
        area->monster = mansionMonsters[random];
    else if(type == CAVE)
        area->monster = caveMonster;

    return area;
}


void freeDungeon(Dungeon* dungeon) {
    Room* current = dungeon->start;
    while(current != NULL) {
        Room* tmp = current->nextRoom;
        free(current);
        current = tmp;
    }
    free(dungeon);
}
