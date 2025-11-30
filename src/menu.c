#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "utils.h"
#include "game.h"
#include "global.h"

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

    clearScreen();
    drawTitle("MISSIONI");
    puts("1. Palude Putrescente");
    puts("2. Magione Infestata");
    puts("3. Grotta di Cristallo");
    printf("Seleziona una delle opzioni [1-3]: ");
    choice = readOption("123");

    switch (choice) {
        case '1':
            swampDungeon();
            break;
        case '2':
            mansionDungeon();
            break;
        case '3':
            caveDungeon();
            break;
        default:
            break;
    }

}

void rest() {
    clearScreen();
    HERO.hp = MAX_HP;
    printf("Dopo un riposo accanto a un falo l'eroe %s è tornato in piene forze\n", HERO.name);
    puts("I punti vita sono stati riprestinati");
    printf("Premi un tasto per proseguire...");
    clearInput(); 
}

void inventoryMenu() {    
    char choice;
    clearScreen();
    playerStats();

    printf("Possiedi %d pozioni curative\n", HERO.potions);
    printf("%s la spada potenziata\n", HERO.hasDmgBuff ? "Possiedi" : "Non possiedi");
    printf("%s l'armatura\n", HERO.hasArmor ? "Possiedi" : "Non possiedi");
    printf("%s la spada dell'eroe\n", HERO.hasHeroSword ? "Possiedi" : "Non possiedi");
    printf("%s la chiave del castello del Signor Oscuro\n", HERO.hasCastleKey ? "Possiedi" : "Non possiedi");
    printf("Vuoi usare una pozione curativa? S/N: ");
    while(1) {
        choice = readOption("SNsn");
        if(choice == 'S' || choice == 's') {
            if(HERO.potions > 0) {
                HERO.hp += rand() % 6;
                if(HERO.hp > 20) HERO.hp = 20;
                HERO.potions--;
                printf("Vuoi usare una pozione curativa? S/N: ");
                continue;
            }
            else {
                printf("Non hai pozioni curative da usare...");
                clearInput();
            }
        }
        break;
    }


}

void shopMenu() {

}

void saveGame() {

}

void missionMenu() {
    puts("1. Esplora la stanza del Dungeon");
    puts("2. Negozio");
    puts("3. Inventario");
    puts("4. Torna al Villaggio ( Paga 50 Monete )");
    printf("Seleziona una delle opzioni del menu [1-4]: ");
}
