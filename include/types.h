#ifndef TYPE
#define TYPE

#define NAMESIZE 20
#define QUESTS 3
#define MAX_HP 20
#include <stdbool.h>

typedef struct {
    char name[NAMESIZE];
    int hp;
    int coins;
    int potions;
    bool hasDmgBuff;
    bool hasArmor;
    bool hasCastleKey;
    bool hasHeroSword;
    bool missionComplete[QUESTS];
} Player;


#endif