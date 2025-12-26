/**
 * @file menu.h
 * @brief Sistema di menu e navigazione del gioco.
 */

#ifndef MENU
#define MENU

#include "types.h"

/**
 * @brief Gestisce il menu principale del gioco
 * @param player Puntatore al giocatore
 * @param saves Puntatore alla struttura dei salvataggi
 */
void menu(Player *player, GameSaves *saves);

/**
 * @brief Gestisce il menu dei trucchi (cheat)
 * @param player Puntatore al giocatore
 */
void cheatMenu(Player *player);

/**
 * @brief Inizia una nuova partita
 * @param player Puntatore al giocatore
 * @param saves Puntatore alla struttura dei salvataggi
 */
void newGame(Player *player, GameSaves *saves);

/**
 * @brief Carica una partita salvata
 * @param player Puntatore al giocatore
 * @param saves Puntatore alla struttura dei salvataggi
 * @param fromCheatMenu Indica se chiamato dal menu trucchi
 */
void loadGame(Player *player, GameSaves *saves, bool fromCheatMenu);

/**
 * @brief Gestisce il menu del villaggio
 * @param player Puntatore al giocatore
 * @param saves Puntatore alla struttura dei salvataggi
 */
void villageMenu(Player *player, GameSaves *saves);

/**
 * @brief Gestisce il menu di selezione dei dungeon
 * @param player Puntatore al giocatore
 */
void dungeonMenu(Player *player);

/**
 * @brief Gestisce il menu del negozio
 * @param player Puntatore al giocatore
 */
void shopMenu(Player *player);

/**
 * @brief Salva la partita corrente
 * @param player Puntatore al giocatore
 * @param saves Puntatore alla struttura dei salvataggi
 */
void saveGame(Player *player, GameSaves *saves);

/**
 * @brief Gestisce il menu dell'inventario
 * @param player Puntatore al giocatore
 */
void inventoryMenu(Player *player);

#endif