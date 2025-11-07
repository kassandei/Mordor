#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "utils.h"
#include "game.h"
#include "global.h"

void menu() {
    char ch;

    while(1) {  
        clearScreen();
        drawTitle("Il gioco RPG ASCII text");
        puts("1. Inzia una nuova partita");
        puts("2. Carica un salvataggio");
        puts("3. Esci");
        
        printf("Seleziona una delle opzioni [1-3]: ");
        ch = readOption("123");
        
        clearInput();
        switch (ch) {
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
    char ch;

    while(1) {  
        clearScreen();
        drawTitle("VILLAGIO");
        puts("1. Inizia una missione");
        puts("2. Riposati");
        puts("3. Inventario");
        puts("4. Salva partita");
        puts("5. Torna al menu di gioco");
        playerStats();
        printf("Seleziona una delle opzioni [1-5]: ");
        ch = readOption("12345");
        clearInput();
        switch (ch) {
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
            char confirm;
            while((confirm = getchar()) != 'S' && confirm != 'N') {
                clearInput();
                puts("Opzione non valida riprova: "); 
            }
            clearInput();
            if(confirm == 'S') {
                return;  // torna al menu
            }
            break;
        default:
            break;
        }
    }
}

void dungeonMenu() {
}

void rest() {
    clearScreen();
    HERO.hp = MAX_HP;
    printf("Dopo un riposo accanto a un falo l'eroe %s è tornato in piene forze\n", HERO.name);
    puts("I punti vita sono stati riprestinati");
    clearInput();
}

void inventoryMenu() {
    int ch;
    playerStats();
    printf("Possiedi %d pozioni curative\n", HERO.potions);
    printf(HERO.hasDmgBuff ? "Possiedi" : "Non possiedi" " la spada potenziata\n");
    printf(HERO.hasArmor ? "Possiedi" : "Non possiedi" " l'armatura\n");
    printf(HERO.hasHeroSword ? "Possiedi" : "Non possiedi" " la spada dell'erore\n");
    printf(HERO.hasCastleKey ? "Possiedi" : "Non possiedi" " la chiave del castello del Signor Oscuro\n");

    puts("Vuoi usare una pozione curativa? S/N: ");
    //while((ch = getchar()) != "");

}

void shopMenu() {

}

void saveGame() {

}

