#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "menu.h"

int main(void) {
    
    srand(time(NULL));
    Player hero;
    menu(&hero);

    return 0;

}
