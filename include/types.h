#ifndef TYPE
#define TYPE

#define NAMESIZE 20
#define QUESTS 3

typedef struct {
    char name[NAMESIZE];
    int hp;
    int money;
    int items;
    int missionComplete[QUESTS];
} Player;


#endif