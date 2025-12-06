#ifndef TYPE
#define TYPE

#define NAMESIZE 30
#define QUESTS 3
#define MAX_HP 20
#define PRICE_RETURN 50
#define POTION_PRICE 4
#define DMGBUFF_PRICE 5
#define ARMOR_PRICE 10
#define DMGBUFF 1
#define ARMOR_REDUCEDMG -1
#include <stdbool.h>

typedef enum {
    EMPTY,              
    TRAP,
    COMBAT
} RoomType;

typedef enum {
    SWAMP,
    MANSION,
    CAVE
} DungeonType;

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
    bool isAlive;
} Player;

typedef struct {
    char* name;
    int fatalBlow;
    int dmg;
    int coin;
} Monster;

typedef struct {
    char* name;
    int dmg;
} Trap;

typedef struct {
    RoomType type;
    Monster monster;
    Trap trap;
} Room;

typedef struct {
    Room* room;
    int rooms;
    DungeonType mission;
    bool canExit;
} Dungeon;


#endif