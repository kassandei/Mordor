/**
 * @file save.h
 * @brief Gestione del sistema di salvataggio.
 */

#ifndef SAVE
#define SAVE

#include "types.h"

/**
 * @brief Ottiene la data e ora corrente formattata
 * @param date Buffer dove salvare la data formattata
 * @return Puntatore al buffer contenente la data
 */
char* currentDateTime(char date[DATESIZE]);

/**
 * @brief Aggiunge un nuovo salvataggio alla lista
 * @param player Puntatore al giocatore da salvare
 * @param saves Puntatore alla struttura dei salvataggi
 */
void addSave(Player* player, GameSaves* saves);

/**
 * @brief Carica un salvataggio specifico
 * @param player Puntatore dove caricare i dati del giocatore
 * @param saves Puntatore alla struttura dei salvataggi
 * @param index Numero del salvataggio da caricare
 */
void loadSave(Player* player, GameSaves* saves, int index);

/**
 * @brief Visualizza tutti i salvataggi disponibili
 * @param saves Puntatore alla struttura dei salvataggi
 */
void showSaves(GameSaves* saves);

/**
 * @brief Rimuove un salvataggio dalla lista
 * @param saves Puntatore alla struttura dei salvataggi
 * @param index Numero del salvataggio da eliminare
 */
void removeSave(GameSaves* saves, int index);

#endif