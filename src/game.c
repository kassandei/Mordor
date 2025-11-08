#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "utils.h"
#include "menu.h"

// Uso una variabile globale visto che il giocatore è unico e uno solo
// evitando di passare la struttura per ogni funzione come parametro
Player HERO;

static Monster swampMonsters[] = {
    {"Cane Selvaggio", 2, 1, 0},      // fatalBlow, dmg, coin
    {"Goblin", 3, 2, 2},
    {"Scheletro", 4, 2, 4},
    {"Orco", 3, 4, 6},
    {"Generale Orco", 6, 3, 12}
};

static Trap swampTrap = {
    .name = "Acquitrino Velenoso",
};


void initGame() {
    srand(time(NULL)); // Inizializza il generatore casuale
    HERO.hp = MAX_HP;
    HERO.coins = 50;
    HERO.potions = 0;
    HERO.hasDmgBuff = false;
    HERO.hasArmor = false;
    HERO.hasCastleKey = false;
    HERO.hasHeroSword = false;
    for(int i = 0; i < QUESTS; i++) {
        HERO.missionComplete[i] = false;
    }
}

Room* generateRoom(Dungeon* dungeon) {
    Room *area = (Room*)malloc(sizeof(Room));
    area->type = rand() % 3;
    if(area->type == EMPTY)
        return NULL;
    if(area->type == TRAP) {
        if(dungeon->mission == SWAMP) {
            area->trap.dmg = rand() % 6 + 1;        
        }
        return area;
    }

}


void swampDungeon() {
    Dungeon swamp;
    int obj = 3;
    int nRoom = 0;
    swamp.mission = SWAMP;
    swamp.canExit = false;
    char choice;
    while(1) {
        clearScreen();
        drawTitle("Palude Putrescente");
        puts("Obiettivo : Eliminare 3 Generali Orco");
        printf("Stato di avanzamento : Generale Orco %d/3", 0);
        printf("Stanza numero %d", swamp.rooms + 1);
        playerStats();
        missionMenu();
        choice = readOption("1234");
        
        switch (choice) {
            case '1':
                clearScreen();
                swamp.room = generateRoom(&swamp);
                if(swamp.room == NULL) {
                    puts("La stanza è vuota...");
                    swamp.rooms++; 
                }
                if(swamp.room->type == TRAP) { 
                    printf("Sei caduto nella trappola %s", swampTrap.name);
                    printf("Hai subito %d", swampTrap.dmg);
                }
                free(swamp.room);
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
                returnHome(PRICE_RETURN);
                break;
            default:
                break;
        }
    }
}

void mansionDungeon() {
    char choice;
    
    while(1) {
        clearScreen();
        drawTitle("Magione Infestata");
        puts("Obiettivo : Recupera la chiave del Castello del Signore Oscuro, e sconfiggi un Vampiro Superiore.");
        printf("Stato di avanzamento : Chiave %d/1 | Vampiro Superiore %d/1", 0,0);
        playerStats();
        missionMenu();
        choice = readOption("1234");
        
        switch (choice) {
            case '1':
                /* code */
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
                returnHome(PRICE_RETURN);
                break;
            default:
                break;
        }
    }
}

void caveDungeon() {
    char choice;
    

    while(1) {
        clearScreen();
        drawTitle("Grotta di Cristallo");
        puts("Obiettivo : Recupera la spada del’Eroe.");
        printf("Stato di avanzamento : spada %d/1", 0);
        playerStats();
        missionMenu();
        choice = readOption("1234");
        
        switch (choice) {
            case '1':
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
                returnHome(PRICE_RETURN);
                break;
            default:
                break;
        }
    }
}

