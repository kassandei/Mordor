#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "types.h"
#include "global.h"
#include "save.h"

char* currentDateTime(char date[DATESIZE]) {
    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    strftime(date, DATESIZE, "%Y-%m-%d %H:%M:%S", localTime);

    return date;
}

void addSave() {
    SaveFile* save = (SaveFile*)malloc(sizeof(SaveFile));
    if (!save) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    int index = (SAVES.end == NULL) ? 1 : SAVES.end->number + 1;
    save->number = index;
    currentDateTime(save->date);
    save->player = HERO;
    save->nextSaveFile = NULL;
    
    if(SAVES.start == NULL) {
        SAVES.start = save;
        SAVES.end = save;
    } else {
        SAVES.end->nextSaveFile = save;
        SAVES.end = save;
    }
}

void loadSave(int index) {
    SaveFile* tmp = SAVES.start;
    while(tmp != NULL) {
        if(tmp->number == index) {
            HERO = tmp->player;
            printf("Salvataggio %d caricato con successo", index);
            return;
        }
        tmp = tmp->nextSaveFile;
    }
    printf("Non esiste il salvataggio numero : %d ...riprova", index);
}

void showSaves() {
    SaveFile* tmp = SAVES.start;
    while(tmp != NULL) { 
        printf("%d. %s | %10s | %2d HP | %5d MONETE | %3d OGGETTI | %d MISSIONI COMPLETATE\n",
                tmp->number, tmp->date, tmp->player.name, tmp->player.hp, tmp->player.coins,
                tmp->player.potions, missionCompleted(&tmp->player));
        tmp = tmp->nextSaveFile;
    }
}

void removeSave(int index) {
    SaveFile* tmp = SAVES.start;
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
        SAVES.start = tmp->nextSaveFile;
        if(SAVES.start == NULL) {
            SAVES.end = NULL;
        }
    } else {
        prev->nextSaveFile = tmp->nextSaveFile;
        if(tmp == SAVES.end) {
            SAVES.end = prev;
        }
    }
    
    free(tmp);
    
    // Rinumera i salvataggi successivi
    tmp = SAVES.start;
    int currentNumber = 1;
    while(tmp != NULL) {
        tmp->number = currentNumber++;
        tmp = tmp->nextSaveFile;
    }    
    printf("Salvataggio %d eliminato con successo", index);
}