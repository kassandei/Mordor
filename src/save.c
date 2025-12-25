#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "types.h"
#include "save.h"

char* currentDateTime(char date[DATESIZE]) {
    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    strftime(date, DATESIZE, "%Y-%m-%d %H:%M:%S", localTime);

    return date;
}

void addSave(Player *player, GameSaves *saves) {
    SaveFile* save = (SaveFile*)malloc(sizeof(SaveFile));
    if (!save) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
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

void loadSave(Player *player, GameSaves *saves, int index) {
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

void showSaves(GameSaves *saves) {
    SaveFile* tmp = saves->start;
    while(tmp != NULL) { 
        printf("%d. %s | %10s | %2d HP | %5d MONETE | %3d OGGETTI | %d MISSIONI COMPLETATE\n",
                tmp->number, tmp->date, tmp->player.name, tmp->player.hp, tmp->player.coins,
                tmp->player.inventory.potions, missionCompleted(&tmp->player));
        tmp = tmp->nextSaveFile;
    }
}

void removeSave(GameSaves *saves, int index) {
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
    
    // Se è il primo elemento
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
    
    // Rinumera i salvataggi successivi
    tmp = saves->start;
    int currentNumber = 1;
    while(tmp != NULL) {
        tmp->number = currentNumber++;
        tmp = tmp->nextSaveFile;
    }    
    printf("Salvataggio %d eliminato con successo", index);
}