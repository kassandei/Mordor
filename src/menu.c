#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "global.h"
#include "utils.h"
#include "game.h"

void menu() {
    clearScreen();
    char ch;
    drawTitle("Il gioco RPG ASCII text");
    puts("1. Inzia una nuova partita");
    puts("2. Carica un salvataggio");
    puts("3. Esci");
    
    printf("Seleziona una delle opzioni [1-3]: ");
    
    while((ch = getchar()) != '1' && ch != '2' && ch != '3') {
        clearInput();
        printf("Opzione non valida riprova: ");
    }


    switch (ch)
    {
    case '1':
        newGame();
        break;
    case '2':
        loadGame();
    case '3':
        exit(EXIT_SUCCESS);
    default:
        break;
    }
}

void newGame() {
    clearScreen();
    story();
    puts("Inserisci il tuo nome:");
    readString(HERO.name, NAMESIZE);
    initGame();
    villageMenu();
}

void loadGame() {
    puts("Carica partita");
}

void villageMenu() {

}