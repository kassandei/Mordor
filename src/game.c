#include <stdio.h>
#include "game.h"

void initGame(Player *hero) {
    hero->hp = 20;
    hero->coins = 0;
    hero->potions = 0;
    for(int i = 0; i < QUESTS; i++) {
        hero->missionComplete[i] = 0;
    }
}


