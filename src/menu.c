#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "utils.h"
#include "game.h"
#include "global.h"
#include "types.h"

void menu() {
    char choice;

    while(1) {  
        clearScreen();
        drawTitle("Il gioco RPG ASCII text");
        puts("1. Inzia una nuova partita");
        puts("2. Carica un salvataggio");
        puts("3. Esci");
        
        printf("Seleziona una delle opzioni [1-3]: ");
        choice = readOption("123");
        
        switch (choice) {
            case '1':
                newGame();
                break;
            case '2':
                loadGame();
                break;
            case '3':
                exit(EXIT_SUCCESS);
            default:
                break;
        }
    }
}

void newGame() {
    clearScreen();
    story();
    printf("Inserisci il tuo nome: ");
    readString(HERO.name, NAMESIZE);
    initGame();
    villageMenu();
}

void loadGame() {
    puts("Carica partita");
}

void villageMenu() {
    char choice;

    while(HERO.isAlive) {  
        clearScreen();
        drawTitle("VILLAGIO");
        puts("1. Inizia una missione");
        puts("2. Riposati");
        puts("3. Inventario");
        puts("4. Salva partita");
        puts("5. Torna al menu di gioco");
        playerStats();
        printf("Seleziona una delle opzioni [1-5]: ");
        choice = readOption("12345");
        switch (choice) {
            case '1':
                dungeonMenu();
                break;
            case '2': 
                rest();
                break;
            case '3':
                inventoryMenu();
                break;
            case '4': 
                saveGame();
                break;
            case '5':
                clearScreen();
                printf("Sei sicuro di voler uscire? Perderai i progressi non salvati\nS/N : ");
                choice = readOption("SN");
                if(choice == 'S') {
                    return;  // torna al menu
                }
                break;
            default:
                break;
        }
    }
}

void dungeonMenu() {
    char choice;
    while(HERO.isAlive) {
        clearScreen();
        drawTitle("MISSIONI");
        
        if(missionCompleted() == 3) {
            puts("Hai completato tutte le missioni sei pronto ad affrontare il Signor Oscuro? S/N: ");
            choice = readOption("SN");
            if(choice == 'S') {
                bossMenu();
            } 
            return;
        }
        printf("1. Palude Putrescente %s\n", HERO.missionComplete[SWAMP] ? "[COMPLETATA]" : "");
        printf("2. Magione Infestata %s\n", HERO.missionComplete[MANSION] ? "[COMPLETATA]" : "");
        printf("3. Grotta di Cristallo %s\n", HERO.missionComplete[CAVE] ? "[COMPLETATA]" : "");
        printf("Seleziona una delle opzioni [1-3]: ");
        choice = readOption("123");

        switch (choice) {
            case '1':
                if(isCompleted(SWAMP)) { 
                    puts("Missione già completata"); 
                    clearInput();
                    continue; 
                }
                swampDungeon();
                return;
            case '2':
                if(isCompleted(MANSION)) { 
                    puts("Missione già completata"); 
                    clearInput();
                    continue; 
                }
                mansionDungeon();
                return;
            case '3':
                if(isCompleted(CAVE)) {
                    puts("Missione già completata"); 
                    clearInput();
                    continue; 
                }
                caveDungeon();
                return;
            default:
                break;
        }
    }
}

void bossMenu() {
    clearInput();
}

void inventoryMenu() {    
    char choice;
    int restoreHP;

    while(1) {
        clearScreen();
        playerStats();

        printf("Possiedi %d pozioni curative\n", HERO.potions);
        printf("%s la spada potenziata\n", HERO.hasDmgBuff ? "Possiedi" : "Non possiedi");
        printf("%s l'armatura\n", HERO.hasArmor ? "Possiedi" : "Non possiedi");
        printf("%s la spada dell'eroe\n", HERO.hasHeroSword ? "Possiedi" : "Non possiedi");
        printf("%s la chiave del castello del Signor Oscuro\n", HERO.hasCastleKey ? "Possiedi" : "Non possiedi");
        
        if(HERO.potions == 0) {
            printf("\nNon hai pozioni curative da usare...\n");
            clearInput();
            break;
        }
        
        printf("\nVuoi usare una pozione curativa? S/N: ");
        choice = readOption("SNsn");
        
        if(choice == 'S' || choice == 's') {
            if(HERO.hp >= MAX_HP) {
                printf("\nHai già i punti vita al massimo!\n");
                clearInput();
                break;
            }
            int hpBefore = HERO.hp;
            restoreHP = 1 + (rand() % 6);  
            HERO.hp += restoreHP;
            
            if(HERO.hp > MAX_HP) {
                restoreHP = MAX_HP - hpBefore;
                HERO.hp = MAX_HP;
            }
            
            HERO.potions--;
            printf("\nHai ripristinato %d punti vita!\n", restoreHP);
            clearInput();
        } else {
            break;
        }
    }
}
void shopMenu() {
    char choice;

    while(1) {
        clearScreen();
        drawTitle("NEGOZIO");
        puts("Benvenuto nel negozio avventuriero!\n");
        printf(" | OGGETTI            | DESCRIZIONE                     | POSSEDUTI       | COSTO\n");
        printf("-|--------------------|---------------------------------|-----------------|------\n");
        printf("1| Pozione curativa   | Ripristina fino a 6 Punti Vita  | %15d | %d\n", HERO.potions, POTION_PRICE);
        printf("2| Spada potenziata   | +1 all'attacco dell'eroe        | %15s | %d\n", HERO.hasDmgBuff ? "POSSIEDI" : "NON POSSIEDI", DMGBUFF_PRICE);
        printf("3| Armatura           | -1 al danno del nemico/trappola | %15s | %d\n", HERO.hasArmor ? "POSSIEDI" : "NON POSSIEDI", ARMOR_PRICE);
        printf("4| Esci\n");
        printf("\nMonete disponibili: %d\n", HERO.coins);
        printf("Seleziona una delle opzioni [1-4]: ");
        choice = readOption("1234");
        switch (choice) {
            case '1':
                if(HERO.coins >= POTION_PRICE) {
                    HERO.coins -= POTION_PRICE;
                    HERO.potions++;
                    puts("Hai acquistato una pozione!");
                } else {
                    puts("Non hai abbastanza soldi...");
                }
                clearInput();
                break;
            case '2':
                if(HERO.hasDmgBuff) {
                    puts("Possiedi già il potenziamento alla spada");
                } else if(HERO.coins >= DMGBUFF_PRICE) {
                    HERO.hasDmgBuff = true;
                    HERO.coins -= DMGBUFF_PRICE;
                    puts("Hai acquistato il potenziamento alla spada!");
                } else {
                    puts("Non hai abbastanza soldi...");
                }
                clearInput();
                break;
            case '3':
                if(HERO.hasArmor) {
                    puts("Possiedi già l'armatura");
                } else if(HERO.coins >= ARMOR_PRICE) {
                    HERO.hasArmor = true;
                    HERO.coins -= ARMOR_PRICE;
                    puts("Hai acquistato l'armatura!");
                } else {
                    puts("Non hai abbastanza soldi...");
                }
                clearInput();
                break;
            case '4':
                return;
        }
    }
}

void saveGame() {

}

