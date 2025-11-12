#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "utils.h"
#include "menu.h"
#include "dungeon.h"

Player HERO;

void initGame() {
    HERO.hp = MAX_HP;
    HERO.coins = 0;
    HERO.potions = 0;
    HERO.hasDmgBuff = false;
    HERO.hasArmor = false;
    HERO.hasCastleKey = false;
    HERO.hasHeroSword = false;
    HERO.isAlive = true;
    for(int i = 0; i < QUESTS; i++) {
        HERO.missionComplete[i] = false;
    }
}

void swampDungeon() {
    Dungeon dungeon;
    int obj = 0;
    dungeon.rooms = 0;
    dungeon.mission = SWAMP;
    dungeon.canExit = false;
    char choice;
    while(HERO.isAlive) {
        clearScreen();
        drawTitle("Palude Putrescente");
        puts("Obiettivo : Eliminare 3 Generali Orco");
        printf("Stato di avanzamento : Generale Orco %d/3\n", obj);
        printf("Stanza numero %d\n", dungeon.rooms + 1);
        playerStats();
        missionMenu();
        choice = readOption("1234");
        
        switch (choice) {
            case '1':
                clearScreen();
                dungeon.room = generateRoom(&dungeon);
                if(dungeon.room->type == TRAP) { 
                    printf("Sei caduto nella trappola %s\n", dungeon.room->trap.name);
                    printf("Hai subito %d", dungeon.room->trap.dmg);
                    HERO.hp -= dungeon.room->trap.dmg;
                    if(HERO.hp <= 0) gameOver();
                }
                else combat(&dungeon.room->monster);
                dungeon.rooms++; 
                free(dungeon.room);
                clearInput();
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
    
    while(HERO.isAlive) {
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
    

    while(HERO.isAlive) {
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

int rollDice() {
    return (rand() % 6 + 1);
}

void gameOver() {
    printf("\nSei stato sconfitto!\n");
    HERO.isAlive = false;
    clearInput();
}

void combat(Monster* monster) {
    printf("Hai incontrato %s\n", monster->name);
    while(1) {
        int dice = rollDice();
        printf("Premi un tasto per tirare il dado...");
        clearInput();
        printf("Dal tiro del dado è uscito %d\n", dice);
        if(dice >= monster->fatalBlow) {
            HERO.coins += monster->coin;
            printf("Hai battuto il nemico! (%d >= %d)\n", dice, monster->fatalBlow);
            printf("Hai ottenuto %d monete\n", monster->coin);
            printf("Premi un tasto per uscire...");
            break;
        }
        else {
            printf("Hai subito %d danni! (%d < %d)\n", monster->dmg, dice, monster->fatalBlow);
            HERO.hp -= monster->dmg;
            if(HERO.hp <= 0) {
                gameOver();
                break;
            }
        }
    }
}