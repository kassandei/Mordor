#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "menu.h"
#include "types.h"

int main(void) {
    Player hero;
    GameSaves saves;
    
    // Inizializza il generatore di numeri casuali
    srand(time(NULL));
    initGame(&hero);
    initGameSaves(&saves);
    menu(&hero, &saves);
    freeGameSaves(&saves);

    return 0;
}
