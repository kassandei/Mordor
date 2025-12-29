/**
 * @file dungeon.h
 * @brief Generazione e gestione delle stanze dei dungeon.
 */

#ifndef DUNGEON
#define DUNGEON


#include "types.h"

#define DUNGEON_ROOMS 10

#define SWAMP_ORC 3
#define SWAMP_MONSTERS 5
#define MANSION_MONSTERS 5
#define CAVE_TRAPS 4

extern Monster swampMonsters[SWAMP_MONSTERS];
extern Monster mansionMonsters[MANSION_MONSTERS];
extern Monster caveMonster;

/**
 * @brief Inizializza un nuovo dungeon vuoto
 * @param dungeon Puntatore al dungeon da inizializzare
 * @return Puntatore al dungeon inizializzato
 */
Dungeon* generateDungeon(Dungeon* dungeon);

/**
 * @brief Aggiunge una nuova stanza al dungeon
 * @param dungeon Puntatore al dungeon
 * @return Puntatore alla stanza creata
 */
Room* addRoom(Dungeon* dungeon);

/**
 * @brief Genera una stanza casuale in base al tipo di dungeon
 * @param dungeon Puntatore al dungeon
 * @return Puntatore alla stanza generata
 */
Room* generateRoom(Dungeon* dungeon);

/**
 * @brief Genera una stanza specifica per il dungeon della palude
 * @param room Puntatore alla stanza da configurare
 * @return Puntatore alla stanza configurata
 */
Room* generateRoomSwamp(Room* room);

/**
 * @brief Genera una stanza specifica per il dungeon della magione
 * @param room Puntatore alla stanza da configurare
 * @return Puntatore alla stanza configurata
 */
Room* generateRoomMansion(Room* room);

/**
 * @brief Genera una stanza specifica per il dungeon della grotta
 * @param room Puntatore alla stanza da configurare
 * @return Puntatore alla stanza configurata
 */
Room* generateRoomCave(Room* room);

/**
 * @brief Configura una stanza come trappola
 * @param area Puntatore alla stanza
 * @param type Tipo di dungeon
 * @param random Indice casuale per la selezione della trappola
 * @return Puntatore alla stanza configurata
 */
Room* trapRoom(Room* area, DungeonType type, int random);

/**
 * @brief Configura una stanza come combattimento
 * @param area Puntatore alla stanza
 * @param type Tipo di dungeon
 * @param random Indice casuale per la selezione del mostro
 * @return Puntatore alla stanza configurata
 */
Room* combatRoom(Room* area, DungeonType type, int random);

/**
 * @brief Libera la memoria allocata per il dungeon
 * @param dungeon Puntatore al dungeon da liberare
 */
void freeDungeon(Dungeon* dungeon);

#endif
