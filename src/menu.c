#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "utils.h"
#include "game.h"
#include "global.h"
#include "types.h"
#include "save.h"

void menu() {
    char choice;

    while(1) {  
        clearScreen();
        drawTitle("Il gioco RPG ASCII text");
        puts("1. Inzia una nuova partita");
        puts("2. Carica un salvataggio");
        puts("3. Esci");
        if(HERO.konamiCode) {
            puts("4. TRUCCHI");
            printf("Seleziona una delle opzioni [1-4]: ");
            choice = readOption("1234");
        }
        else {
            printf("Seleziona una delle opzioni [1-3]: ");
            choice = readOption("123w");
        }
        
        switch (choice) {
            case '1':
                newGame();
                break;
            case '2':
                loadGame(false);
                break;
            case '3':
                freeGameSaves();
                exit(EXIT_SUCCESS);
            case '4':
                loadGame(true);
                break;
            case 'w':
                HERO.konamiCode = konamiCode();
                break;
        }
    }
}

void cheatMenu() {
    char hpInput[NAMESIZE];
    char coinInput[NAMESIZE];
    char choice;

    while(1) {
        clearScreen();
        drawTitle("MENU TRUCCHI");
        printf("Giocatore: %s\n", HERO.name);
        printf("HP attuali: %d\n", HERO.hp);
        printf("Monete attuali: %d\n", HERO.coins);
        printf("Chiave castello: %s\n\n", HERO.hasCastleKey ? "SI" : "NO");
        
        puts("1. Modifica HP");
        puts("2. Modifica Monete");
        puts("3. Sblocca tutte le missioni e la chiave del castello");
        puts("4. Vai al villaggio");
        printf("Seleziona un'opzione [1-4]: ");
        choice = readOption("1234");
        
        switch(choice) {
            case '1':
                printf("\nInserisci nuovi HP [1-9999]: ");
                readString(hpInput, NAMESIZE);
                int newHP = atoi(hpInput);
                if(newHP > 0 && newHP <= 9999) {
                    HERO.hp = newHP;
                    printf("HP modificati con successo!");
                } else {
                    printf("Valore non valido!");
                }
                clearInput();
                break;
                
            case '2':
                printf("\nInserisci nuove monete [0-9999]: ");
                readString(coinInput, NAMESIZE);
                int newCoins = atoi(coinInput);
                if(newCoins >= 0 && newCoins <= 9999) {
                    HERO.coins = newCoins;
                    printf("Monete modificate con successo!");
                } else {
                    printf("Valore non valido!");
                }
                clearInput();
                break;
                
            case '3':
                HERO.hasCastleKey = true;
                for(int i = 0; i < QUESTS; i++) {
                    HERO.missionComplete[i] = true;
                }
                printf("Tutte le missioni sbloccate e chiave del castello ottenuta!");
                clearInput();
                break;               
            case '4':
                return;
        }
    }
}

void newGame() {
    clearScreen();
    story();
    initGame();
    printf("Inserisci il tuo nome: ");
    readString(HERO.name, NAMESIZE);
    villageMenu();
}

void loadGame(bool fromCheatMenu) {
    char saveNumber[NAMESIZE];
    int selectedSave;
    char choice;

    while(1) {
        clearScreen();
        drawTitle("SALVATAGGI");
        if(SAVES.start == NULL) {
            printf("Non ci sono salvataggi presenti in memoria...");
            clearInput();
            return;
        }
        showSaves();
        printf("\nSeleziona un salvataggio [1-%d]: ", SAVES.end->number);
        readString(saveNumber, NAMESIZE);
        selectedSave = atoi(saveNumber);
        while(selectedSave <= 0 || selectedSave > SAVES.end->number) {
            printf("Salvataggio non valido inserisci un numero valido: ");
            readString(saveNumber, NAMESIZE);
            selectedSave = atoi(saveNumber);
        }

        printf("\nSeleziona un'opzione per il salvataggio %d:\n", selectedSave);
        puts("1. Carica");
        puts("2. Elimina");
        puts("3. Esci");
        if(fromCheatMenu) {
            puts("4. Carica e modifica");
            printf("Seleziona una delle opzioni [1-4]: ");
            choice = readOption("1234");
        }
        else {
            printf("Seleziona una delle opzioni [1-3]: ");
            choice = readOption("123");
        }

        switch (choice)
        {
            case '1':
                loadSave(selectedSave);
                clearInput();
                villageMenu();
                return;
            case '2':
                removeSave(selectedSave);
                clearInput();
                break;
            case '3': 
                return;
            case '4':
                loadSave(selectedSave);
                clearInput();
                cheatMenu();
                villageMenu();
                return;
            default:
                break;
        }
    }
}

void saveGame() {
    addSave();
    printf("Partita salvata...");
    clearInput();
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
                printf("Stai per tornare al menu principale del gioco, "
                       "vuoi salvare l'attuale partita? S/N : ");
                choice = readOption("SN");
                if(choice == 'S') {
                    addSave();
                }
                return;  // torna al menu
                break;
            default:
                break;
        }
    }
}

void dungeonMenu() {
    char choice;

    while (HERO.isAlive) {
        clearScreen();
        drawTitle("MISSIONI");

        if (missionCompleted(&HERO) == 3) {
            puts("Hai completato tutte le missioni. Affrontare il Signor Oscuro? S/N:");
            if (readOption("SN") == 'S')    bossFight();
            return;
        }

        if (!isCompleted(SWAMP))
            printf("1. Palude Putrescente\n");
        if (!isCompleted(MANSION))
            printf("2. Magione Infestata\n");
        if (!isCompleted(CAVE))
            printf("3. Grotta di Cristallo\n");

        printf("Seleziona una delle opzioni [1-3]: ");
        choice = readOption("123");

        if (choice == '1' && !isCompleted(SWAMP))   { swampDungeon();   return; }
        if (choice == '2' && !isCompleted(MANSION)) { mansionDungeon(); return; }
        if (choice == '3' && !isCompleted(CAVE))    { caveDungeon();    return; }
    }
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
        puts("+====+========================+=================================+=================+=======+");
        puts("|    | OGGETTO                | DESCRIZIONE                     | POSSEDUTO       | COSTO |");
        puts("+====+========================+=================================+=================+=======+");
        printf("| 1  | Pozione curativa       | Ripristina fino a 6 Punti Vita  | %15d | %5d |\n", HERO.potions, POTION_PRICE);
        puts("+----+------------------------+---------------------------------+-----------------+-------+");
        printf("| 2  | Spada potenziata       | +1 all'attacco dell'eroe        | %15s | %5d |\n", HERO.hasDmgBuff ? "SI" : "NO", DMGBUFF_PRICE);
        puts("+----+------------------------+---------------------------------+-----------------+-------+");
        printf("| 3  | Armatura               | -1 al danno nemico/trappola     | %15s | %5d |\n", HERO.hasArmor ? "SI" : "NO", ARMOR_PRICE);
        puts("+----+------------------------+---------------------------------+-----------------+-------+");
        puts("| 4  | Esci dal negozio                                                                   |");
        puts("+====+====================================================================================+");
        printf("\nMonete disponibili: %d\n", HERO.coins);
        printf("\nSeleziona una delle opzioni [1-4]: ");
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


