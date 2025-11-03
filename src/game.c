#include <stdio.h>
#include "game.h"
#include "global.h"

void initGame() {
    HERO.hp = 20;
    HERO.money = 0;
    HERO.items = 0;
    for(int i = 0; i < QUESTS; i++) {
        HERO.missionComplete[i] = 0;
    }
}


