#ifndef TYPE
#define TYPE

#include <stdbool.h>

#define NAMESIZE 30
#define QUESTS 3
#define MAX_HP 20
#define PRICE_RETURN 50
#define POTION_PRICE 4
#define DMGBUFF_PRICE 5
#define ARMOR_PRICE 10
#define DMGBUFF 1
#define ARMOR_REDUCEDMG -1
#define BOSS_FIGHT_ROUNDS 5

typedef enum {
    EMPTY,              
    TRAP,
    COMBAT
} RoomType;

typedef enum {
    SHIELD,
    MAGIC,
    SWORD
} Move;

typedef enum {
    HEAD, 
    TAIL
} CoinFace;

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
    bool konamiCode;
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
    int coin;
} Trap;

typedef struct Room {
    int roomNumber;
    RoomType type;
    Monster monster;
    Trap trap;
    struct Room* nextRoom;
} Room;

typedef struct {
    Room* start;
    Room* end;
    DungeonType mission;
    int roomCount;
} Dungeon;

typedef struct {
    int round;
    int win;
    int lose;
} BossRoom;

#endif