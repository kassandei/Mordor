#include <stdio.h>
#include "types.h"

Player HERO;

void initGame() {
    HERO.hp = 20;
    for(int i = 0; i < QUESTS; i++) {
        HERO.missionComplete[i] = 0;
    }
}


