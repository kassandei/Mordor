#ifndef TYPE
#define TYPE

#define NAMESIZE 20
#define QUESTS 3
#define MAX_HP 20
#define PRICE_RETURN 50
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
} Player;

typedef struct {
    char name[NAMESIZE];
    int fatalBlow;
    int dmg;
    int coin;
} Monster;

typedef struct {
    char name[NAMESIZE];
    int dmg;
} Trap;

typedef struct {
    char name[NAMESIZE];
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