#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "global.h"
#include "utils.h"
#include "game.h"

void menu() {
    char ch;

    while(1) {  
        clearScreen();
        drawTitle("Il gioco RPG ASCII text");
        puts("1. Inzia una nuova partita");
        puts("2. Carica un salvataggio");
        puts("3. Esci");
        
        printf("Seleziona una delle opzioni [1-3]: ");
        while((ch = getchar()) != '1' && ch != '2' && ch != '3') {
            clearInput();
            printf("Opzione non valida riprova: ");
        }
        
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
    const char *villageOptions = "123456";
    char ch;

    while(1) {  
        clearScreen();
        drawTitle("VILLAGIO");
        puts("1. Inizia una missione");
        puts("2. Riposati");
        puts("3. Inventario");
        puts("4. Negozio");
        puts("5. Salva partita");
        puts("6. Torna al menu di gioco");
        printf("\n%s | HP: %d | MONEY: %d | MISSION COMPLETED %d/3\n\n", 
                HERO.name, HERO.hp, HERO.money, missionCompleted());

        printf("Seleziona una delle opzioni [1-6]: ");
        while((ch = getchar()) && !strchr(villageOptions, ch)) {
            clearInput();
            printf("Opzione non valida riprova: "); 
        }

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
            shopMenu();
            break;
        case '5': 
            saveGame();
            break;
        case '6':
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

}

void inventoryMenu() { 
}

void shopMenu() {

}

void saveGame() {

}