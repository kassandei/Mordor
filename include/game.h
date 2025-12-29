/**
 * @file game.h
 * @brief Gestione dei dungeon e della logica di combattimento.
 */

#ifndef GAME
#define GAME


#include "types.h"

/**
 * @brief Inizializza un nuovo giocatore con valori di default
 * @param player Puntatore al giocatore da inizializzare
 */
void initGame(Player* player);

/**
 * @brief Inizializza la struttura dei salvataggi
 * @param saves Puntatore alla struttura da inizializzare
 */
void initGameSaves(GameSaves* saves);

/**
 * @brief Libera la memoria allocata per i salvataggi
 * @param saves Puntatore alla struttura dei salvataggi
 */
void freeGameSaves(GameSaves* saves);

/**
 * @brief Gestisce il dungeon della palude
 * @param player Puntatore al giocatore
 */
void swampDungeon(Player* player);

/**
 * @brief Gestisce il dungeon della magione
 * @param player Puntatore al giocatore
 */
void mansionDungeon(Player* player);

/**
 * @brief Gestisce il dungeon della grotta
 * @param player Puntatore al giocatore
 */
void caveDungeon(Player* player);

/**
 * @brief Gestisce lo scontro finale con il boss
 * @param player Puntatore al giocatore
 */
void bossFight(Player* player);

/**
 * @brief Gestisce la sconfitta del giocatore
 * @param player Puntatore al giocatore
 */
void gameOver(Player* player);

/**
 * @brief Gestisce un combattimento standard
 * @param player Puntatore al giocatore
 * @param monster Puntatore al mostro da affrontare
 */
void combat(Player* player, Monster* monster);

/**
 * @brief Gestisce un singolo round del combattimento finale
 * @param player Puntatore al giocatore
 * @param playerMove Mossa scelta dal giocatore
 * @param room Puntatore alla stanza del boss
 */
void finalcombat(Player* player, Move playerMove, BossRoom* room);

#endif