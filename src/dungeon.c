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
    {"Generale Orco", 6, 3, 12}       // Colpo Fatale diventa 5 con Spada dell'Eroe
};

static Trap swampTrap = {
    .name = "Acquitrino Velenoso",
    .dmg = 0,
    .coin = 0
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
    .dmg = 3,
    .coin = 0
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
    {"Forziere Misterioso", 2, 10},
    {"Rupe scoscesa", 6, 0}
};

/**
 * @brief Inizializza una struttura dungeon vuota
 * 
 * Prepara il dungeon impostando i puntatori della lista concatenata a NULL
 * e azzerando il contatore delle stanze per permettere l'aggiunta progressiva
 * di nuove stanze tramite addRoom.
 * 
 * @param dungeon Puntatore alla struttura Dungeon da inizializzare
 * @return Puntatore alla struttura Dungeon inizializzata
 */
Dungeon* generateDungeon(Dungeon* dungeon) {
    dungeon->start = NULL;
    dungeon->end = NULL;
    dungeon->roomCount = 0;
    return dungeon;
}

/**
 * @brief Aggiunge una nuova stanza alla fine del dungeon
 * 
 * Genera una stanza casuale con generateRoom, le assegna un numero progressivo
 * e la inserisce in coda alla lista concatenata. Se è la prima stanza, diventa
 * sia start che end della lista. Il contatore roomCount viene incrementato
 * automaticamente.
 * 
 * @param dungeon Puntatore al dungeon
 * @return Puntatore alla stanza appena creata
 */
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

/**
 * @brief Genera una stanza casuale specifica per il tipo di dungeon
 * 
 * Alloca la memoria per una nuova stanza e passa la configurazione
 * alla funzione appropriata in base al tipo di missione (SWAMP, MANSION o CAVE).
 * 
 * @param dungeon Puntatore al dungeon corrente
 * @return Puntatore alla stanza generata
 */
Room* generateRoom(Dungeon* dungeon) {
    Room *room = (Room*)malloc(sizeof(Room));
    if (!room) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    switch (dungeon->mission) {
        case SWAMP:
            room = generateRoomSwamp(room);
            break;
        case MANSION:
            room = generateRoomMansion(room);
            break;
        case CAVE:
            room = generateRoomCave(room);
            break;
        default:
            break;
    }
    return room;
}

/**
 * @brief Genera una stanza per la palude
 * 
 * Le stanze hanno trappole o mostri casuali. Le trappole nella palude
 * fanno un danno variabile tra 1 e 5.
 * 
 * @param room Puntatore alla stanza da configurare
 * @return Puntatore alla stanza configurata
 */
Room* generateRoomSwamp(Room* room) {
    int roomType = rand() % 2 + 1; // trap o combat
    int monsterIndex = rand() % SWAMP_MONSTERS;
    room->type = roomType;
    if(room->type == TRAP) {
        room = trapRoom(room, SWAMP, rand() % 5 + 1);
    }
    else {
        room = combatRoom(room, SWAMP, monsterIndex);
    }
    return room;
}

/**
 * @brief Genera una stanza per la magione
 * 
 * Ogni stanza può avere una trappola fissa o uno dei cinque nemici della magione
 * 
 * @param room Puntatore alla stanza da configurare
 * @return Puntatore alla stanza configurata
 */
Room* generateRoomMansion(Room* room) {
    int roomType = rand() % 2 + 1; // trap o combat
    int monsterIndex = rand() % MANSION_MONSTERS;
    room->type = roomType;
    if(room->type == TRAP) {
       room = trapRoom(room, MANSION, mansionTrap.dmg);
    }
    else {
        room = combatRoom(room, MANSION, monsterIndex);
    }
    return room;
}

/**
 * @brief Genera una stanza per la grotta
 * 
 * La grotta ha tre tipi di stanze: vuote, con trappole o con mostri.
 * Alcune trappole sono particolari, tipo il forziere che può dare monete
 * o fare danno, e la rupe che fa danni casuali.
 * 
 * @param room Puntatore alla stanza da configurare
 * @return Puntatore alla stanza configurata
 */
Room* generateRoomCave(Room* room) {
    int roomType = rand() % 3; // considera anche la stanza vuota
    int trapIndex = rand() % CAVE_TRAPS;
    room->type = roomType;
    if(room->type == TRAP) {
        room = trapRoom(room, CAVE, trapIndex);
    }
    else {
        room = combatRoom(room, CAVE, -1);
    }
    return room;
}

/**
 * @brief Configura una stanza come trappola
 * 
 * Gestisce i tre tipi di trappole dei vari dungeon. La grotta ha trappole
 * speciali: il forziere lancia una moneta per decidere se dare ricompense
 * o fare danno, la rupe fa danni variabili con un dado.
 * 
 * @param room Puntatore alla stanza
 * @param type Tipo di dungeon
 * @param randomIndex Indice per scegliere la trappola
 * @return Puntatore alla stanza configurata
 */
Room* trapRoom(Room* room, DungeonType type, int randomIndex) {
    if(type == SWAMP) {
        room->trap = swampTrap;
        room->trap.dmg = randomIndex;
    }
    else if(type == MANSION) {
        room->trap = mansionTrap;
        room->trap.dmg = randomIndex;
    }
    else if(type == CAVE) {
        room->trap = caveTraps[randomIndex];
        if(strcmp("Forziere Misterioso", room->trap.name) == 0) {
            // Decide se infliggere danni o guadagnare monete
            CoinFace toss = flipCoin();
            if(toss == HEAD) {
                room->trap.dmg = 0;
            }
            else room->trap.coin = 0;
        }
        else if(strcmp("Rupe scoscesa", room->trap.name) == 0) {
            room->trap.dmg = rollDice();
        }
    }
    return room;
}

/**
 * @brief Configura una stanza come combattimento
 * @param room Puntatore alla stanza
 * @param type Tipo di dungeon
 * @param monsterIndex Indice del mostro da mettere
 * @return Puntatore alla stanza configurata
 */
Room* combatRoom(Room* room, DungeonType type, int monsterIndex) {
    if(type == SWAMP) 
        room->monster = swampMonsters[monsterIndex];
    else if(type == MANSION)
        room->monster = mansionMonsters[monsterIndex];
    else if(type == CAVE)
        room->monster = caveMonster;

    return room;
}

/**
 * @brief Libera tutta la memoria allocata per il dungeon
 * 
 * Attraversa la lista concatenata delle stanze deallocando ciascuna
 * di esse in sequenza per prevenire memory leak, poi libera anche
 * la struttura principale del dungeon.
 * 
 * @param dungeon Puntatore al dungeon da deallocare
 */
void freeDungeon(Dungeon* dungeon) {
    Room* current = dungeon->start;
    while(current != NULL) {
        Room* nextRoom = current->nextRoom;
        free(current);
        current = nextRoom;
    }
    free(dungeon);
}
