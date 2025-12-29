#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "types.h"
#include "save.h"

/**
 * @brief Ottiene la data e ora corrente formattata
 * @param date Buffer dove salvare la data formattata
 * @return Puntatore al buffer contenente la data
 */
char* currentDateTime(char date[DATESIZE]) {
    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    strftime(date, DATESIZE, "%Y-%m-%d %H:%M:%S", localTime);

    return date;
}

/**
 * @brief Aggiunge un nuovo salvataggio alla lista
 * 
 * Crea un nuovo nodo nella lista concatenata dei salvataggi, assegnando
 * automaticamente un numero progressivo e salvando il timestamp corrente.
 * I salvataggi sono mantenuti in ordine cronologico di creazione.
 * 
 * @param player Puntatore al giocatore da salvare
 * @param saves Puntatore alla struttura dei salvataggi
 */
void addSave(Player* player, GameSaves* saves) {
    SaveFile* save = (SaveFile*)malloc(sizeof(SaveFile));
    if (!save) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    
    // se non ci sono salvataggi parti da 1
    int index = (saves->end == NULL) ? 1 : saves->end->number + 1;
    save->number = index;
    currentDateTime(save->date);
    save->player = *player;
    save->nextSaveFile = NULL;
    
    if(saves->start == NULL) {
        saves->start = save;
        saves->end = save;
    } else {
        saves->end->nextSaveFile = save;
        saves->end = save;
    }
}

/**
 * @brief Carica un salvataggio specifico
 * 
 * Cerca nella lista dei salvataggi quello con il numero richiesto e,
 * se trovato, copia tutti i dati del giocatore salvato nella struttura
 * player corrente. Se il salvataggio non esiste, mostra un errore.
 * 
 * @param player Puntatore dove caricare i dati del giocatore
 * @param saves Puntatore alla struttura dei salvataggi
 * @param index Numero del salvataggio da caricare
 */
void loadSave(Player* player, GameSaves* saves, int index) {
    SaveFile* tmp = saves->start;
    while(tmp != NULL) {
        if(tmp->number == index) {
            *player = tmp->player;
            printf("Salvataggio %d caricato con successo", index);
            return;
        }
        tmp = tmp->nextSaveFile;
    }
    
    printf("Non esiste il salvataggio numero : %d ...riprova", index);
}

/**
 * @brief Visualizza tutti i salvataggi disponibili
 * @param saves Puntatore alla struttura dei salvataggi
 */
void showSaves(GameSaves* saves) {
    SaveFile* tmp = saves->start;
    while(tmp != NULL) { 
        printf("%d. %s | %10s | %2d HP | %5d MONETE | %3d OGGETTI | %d MISSIONI COMPLETATE\n",
                tmp->number, tmp->date, tmp->player.name, tmp->player.hp, tmp->player.coins,
                tmp->player.inventory.potions, missionCompleted(&tmp->player));
        tmp = tmp->nextSaveFile;
    }
}

/**
 * @brief Rimuove un salvataggio dalla lista
 * 
 * Elimina il salvataggio specificato dalla lista concatenata e rinumera
 * tutti i salvataggi rimanenti per mantenere una sequenza continua (1, 2, 3...).
 * 
 * Gestisce casi speciali come:
 * - Rimozione del primo elemento
 * - Rimozione dell'ultimo elemento
 * - Rimozione dell'unico elemento (la lista diventa vuota)
 * 
 * @param saves Puntatore alla struttura dei salvataggi
 * @param index Numero del salvataggio da eliminare
 */
void removeSave(GameSaves* saves, int index) {
    SaveFile* tmp = saves->start;
    SaveFile* prev = NULL;
    
    while(tmp != NULL && tmp->number != index) {
        prev = tmp;
        tmp = tmp->nextSaveFile;
    }
    
    if(tmp == NULL) {
        printf("Non esiste il salvataggio numero: %d ...riprova", index);
        return;
    }
    
    if(prev == NULL) {
        saves->start = tmp->nextSaveFile;
        if(saves->start == NULL) {
            saves->end = NULL;
        }
    } else {
        prev->nextSaveFile = tmp->nextSaveFile;
        if(tmp == saves->end) {
            saves->end = prev;
        }
    }
    
    free(tmp);
    
    // Rinumerazione per mantenere sequenza continua
    tmp = saves->start;
    int currentNumber = 1;
    while(tmp != NULL) {
        tmp->number = currentNumber++;
        tmp = tmp->nextSaveFile;
    }    
    
    printf("Salvataggio %d eliminato con successo", index);
}