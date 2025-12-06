#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
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
        printf("Obiettivo : Eliminare %d Generali Orco\n", SWAMP_ORC);
        printf("Stato di avanzamento : Generale Orco %d/3\n", obj);
        printf("Stanza numero %d\n", dungeon.rooms + 1);
        playerStats();
        missionMenu();
        choice = readOption("1234");
        
        switch (choice) {
            case '1':
                clearScreen();
                dungeon.room = generateRoom(&dungeon);
                dungeon.rooms++;
                if (obj < SWAMP_ORC && dungeon.rooms >= DUNGEON_ROOMS - SWAMP_ORC + obj) {
                    dungeon.room->type = COMBAT;
                    dungeon.room->monster = swampMonsters[4];
                }                
                if(dungeon.room->type == TRAP) { 
                    int trapDamage = calculateDamage(dungeon.room->trap.dmg);
                    printf("Sei caduto nella trappola %s\n", dungeon.room->trap.name);
                    printf("Hai subito %d danni\n", trapDamage);
                    HERO.hp -= trapDamage;
                    if(HERO.hp <= 0) gameOver();
                }
                else {
                    combat(&dungeon.room->monster);
                    
                    if(strcmp(dungeon.room->monster.name, "Generale Orco") == 0) {
                        obj++;
                    }
                }
                free(dungeon.room);
                clearInput();
                if(obj == SWAMP_ORC) {
                    clearScreen();
                    puts("Hai completato il dungeon!");
                    HERO.missionComplete[SWAMP] = true;
                    clearInput();
                    return;
                }
                break;
            case '2':
                shopMenu();
                break;
            case '3':
                inventoryMenu();
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
                shopMenu();
                break;
            case '3':
                inventoryMenu();
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
                shopMenu();
                break;
            case '3':
                inventoryMenu();
                break;
            case '4':
                returnHome(PRICE_RETURN);
                break;
            default:
                break;
        }
    }
}

void gameOver() {
    printf("\nSei stato sconfitto!\n");
    HERO.isAlive = false;
}

void combat(Monster* monster) {
    printf("Hai incontrato %s\n", monster->name);
    while(1) {
        int dice = rollDice() + calculateDiceBonus();

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
            int monsterDamage = calculateDamage(monster->dmg);
            printf("Hai subito %d danni! (%d < %d)\n", monsterDamage, dice, monster->fatalBlow);
            HERO.hp -= monsterDamage;
            if(HERO.hp <= 0) {
                gameOver();
                break;
            }
        }
    }
}