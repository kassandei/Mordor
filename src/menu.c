#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "utils.h"
#include "game.h"
#include "types.h"
#include "save.h"

void menu(Player *player, GameSaves *saves) {
    char choice;

    while(1) {  
        clearScreen();
        drawTitle("Il gioco RPG ASCII text");
        puts("1. Inzia una nuova partita");
        puts("2. Carica un salvataggio");
        puts("3. Esci");
        if(player->konamiCode) {
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
                newGame(player, saves);
                break;
            case '2':
                loadGame(player, saves, false);
                break;
            case '3':
                freeGameSaves(saves);
                exit(EXIT_SUCCESS);
            case '4':
                loadGame(player, saves, true);
                break;
            case 'w':
                player->konamiCode = konamiCode();
                break;
        }
    }
}

void cheatMenu(Player *player) {
    char hpInput[NAMESIZE];
    char coinInput[NAMESIZE];
    char choice;

    while(1) {
        clearScreen();
        drawTitle("MENU TRUCCHI");
        printf("Giocatore: %s\n", player->name);
        printf("HP attuali: %d\n", player->hp);
        printf("Monete attuali: %d\n", player->coins);
        printf("Chiave castello: %s\n\n", player->inventory.hasCastleKey ? "SI" : "NO");
        
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
                    player->hp = newHP;
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
                    player->coins = newCoins;
                    printf("Monete modificate con successo!");
                } else {
                    printf("Valore non valido!");
                }
                clearInput();
                break;
                
            case '3':
                player->inventory.hasCastleKey = true;
                for(int i = 0; i < QUESTS; i++) {
                    player->missionComplete[i] = true;
                }
                printf("Tutte le missioni sbloccate e chiave del castello ottenuta!");
                clearInput();
                break;               
            case '4':
                return;
        }
    }
}

void newGame(Player *player, GameSaves *saves) {
    clearScreen();
    story();
    initGame(player);
    printf("Inserisci il tuo nome: ");
    readString(player->name, NAMESIZE);
    villageMenu(player, saves);
}

void loadGame(Player *player, GameSaves *saves, bool fromCheatMenu) {
    char saveNumber[NAMESIZE];
    int selectedSave;
    char choice;

    while(1) {
        clearScreen();
        drawTitle("SALVATAGGI");
        if(saves->start == NULL) {
            printf("Non ci sono salvataggi presenti in memoria...");
            clearInput();
            return;
        }
        showSaves(saves);
        printf("\nSeleziona un salvataggio [1-%d]: ", saves->end->number);
        readString(saveNumber, NAMESIZE);
        selectedSave = atoi(saveNumber);
        while(selectedSave <= 0 || selectedSave > saves->end->number) {
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
                loadSave(player, saves, selectedSave);
                clearInput();
                villageMenu(player, saves);
                return;
            case '2':
                removeSave(saves, selectedSave);
                clearInput();
                break;
            case '3': 
                return;
            case '4':
                loadSave(player, saves, selectedSave);
                clearInput();
                cheatMenu(player);
                villageMenu(player, saves);
                return;
            default:
                break;
        }
    }
}

void saveGame(Player *player, GameSaves *saves) {
    addSave(player, saves);
    printf("Partita salvata...");
    clearInput();
}

void villageMenu(Player *player, GameSaves *saves) {
    char choice;

    while(player->isAlive) {  
        clearScreen();
        drawTitle("VILLAGIO");
        puts("1. Inizia una missione");
        puts("2. Riposati");
        puts("3. Inventario");
        puts("4. Salva partita");
        puts("5. Torna al menu di gioco");
        playerStats(player);
        printf("Seleziona una delle opzioni [1-5]: ");
        choice = readOption("12345");
        switch (choice) {
            case '1':
                dungeonMenu(player);
                break;
            case '2': 
                rest(player);
                break;
            case '3':
                inventoryMenu(player);
                break;
            case '4': 
                saveGame(player, saves);
                break;
            case '5':
                printf("Stai per tornare al menu principale del gioco, "
                       "vuoi salvare l'attuale partita? S/N : ");
                choice = readOption("SN");
                if(choice == 'S') {
                    addSave(player, saves);
                }
                return;  // torna al menu
                break;
            default:
                break;
        }
    }
}

void dungeonMenu(Player *player) {
    char choice;

    while (player->isAlive) {
        clearScreen();
        drawTitle("MISSIONI");

        if (missionCompleted(player) == 3) {
            puts("Hai completato tutte le missioni. Affrontare il Signor Oscuro? S/N:");
            if (readOption("SN") == 'S')    bossFight(player);
            return;
        }

        if (!isCompleted(player, SWAMP))
            printf("1. Palude Putrescente\n");
        if (!isCompleted(player, MANSION))
            printf("2. Magione Infestata\n");
        if (!isCompleted(player, CAVE))
            printf("3. Grotta di Cristallo\n");

        printf("Seleziona una delle opzioni [1-3]: ");
        choice = readOption("123");

        if (choice == '1' && !isCompleted(player, SWAMP))   { swampDungeon(player);   return; }
        if (choice == '2' && !isCompleted(player, MANSION)) { mansionDungeon(player); return; }
        if (choice == '3' && !isCompleted(player, CAVE))    { caveDungeon(player);    return; }
    }
}

void inventoryMenu(Player *player) {    
    char choice;
    int restoreHP;

    while(1) {
        clearScreen();
        playerStats(player);

        printf("Possiedi %d pozioni curative\n", player->inventory.potions);
        printf("%s la spada potenziata\n", player->inventory.hasDmgBuff ? "Possiedi" : "Non possiedi");
        printf("%s l'armatura\n", player->inventory.hasArmor ? "Possiedi" : "Non possiedi");
        printf("%s la spada dell'eroe\n", player->inventory.hasHeroSword ? "Possiedi" : "Non possiedi");
        printf("%s la chiave del castello del Signor Oscuro\n", player->inventory.hasCastleKey ? "Possiedi" : "Non possiedi");
        
        if(player->inventory.potions == 0) {
            printf("\nNon hai pozioni curative da usare...\n");
            clearInput();
            break;
        }
        
        printf("\nVuoi usare una pozione curativa? S/N: ");
        choice = readOption("SNsn");
        
        if(choice == 'S' || choice == 's') {
            if(player->hp >= MAX_HP) {
                printf("\nHai già i punti vita al massimo!\n");
                clearInput();
                break;
            }
            int hpBefore = player->hp;
            restoreHP = 1 + (rand() % 6);  
            player->hp += restoreHP;
            
            if(player->hp > MAX_HP) {
                restoreHP = MAX_HP - hpBefore;
                player->hp = MAX_HP;
            }
            
            player->inventory.potions--;
            printf("\nHai ripristinato %d punti vita!\n", restoreHP);
            clearInput();
        } else {
            break;
        }
    }
}
void shopMenu(Player *player) {
    char choice;

    while(1) {
        clearScreen();
        drawTitle("NEGOZIO");
        puts("Benvenuto nel negozio avventuriero!\n");
        puts("+====+========================+=================================+=================+=======+");
        puts("|    | OGGETTO                | DESCRIZIONE                     | POSSEDUTO       | COSTO |");
        puts("+====+========================+=================================+=================+=======+");
        printf("| 1  | Pozione curativa       | Ripristina fino a 6 Punti Vita  | %15d | %5d |\n", player->inventory.potions, POTION_PRICE);
        puts("+----+------------------------+---------------------------------+-----------------+-------+");
        printf("| 2  | Spada potenziata       | +1 all'attacco dell'eroe        | %15s | %5d |\n", player->inventory.hasDmgBuff ? "SI" : "NO", DMGBUFF_PRICE);
        puts("+----+------------------------+---------------------------------+-----------------+-------+");
        printf("| 3  | Armatura               | -1 al danno nemico/trappola     | %15s | %5d |\n", player->inventory.hasArmor ? "SI" : "NO", ARMOR_PRICE);
        puts("+----+------------------------+---------------------------------+-----------------+-------+");
        puts("| 4  | Esci dal negozio                                                                   |");
        puts("+====+====================================================================================+");
        printf("\nMonete disponibili: %d\n", player->coins);
        printf("\nSeleziona una delle opzioni [1-4]: ");
        choice = readOption("1234");
        switch (choice) {
            case '1':
                if(player->coins >= POTION_PRICE) {
                    player->coins -= POTION_PRICE;
                    player->inventory.potions++;
                    puts("Hai acquistato una pozione!");
                } else {
                    puts("Non hai abbastanza soldi...");
                }
                clearInput();
                break;
            case '2':
                if(player->inventory.hasDmgBuff) {
                    puts("Possiedi già il potenziamento alla spada");
                } else if(player->coins >= DMGBUFF_PRICE) {
                    player->inventory.hasDmgBuff = true;
                    player->coins -= DMGBUFF_PRICE;
                    puts("Hai acquistato il potenziamento alla spada!");
                } else {
                    puts("Non hai abbastanza soldi...");
                }
                clearInput();
                break;
            case '3':
                if(player->inventory.hasArmor) {
                    puts("Possiedi già l'armatura");
                } else if(player->coins >= ARMOR_PRICE) {
                    player->inventory.hasArmor = true;
                    player->coins -= ARMOR_PRICE;
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


