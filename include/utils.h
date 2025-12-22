#ifndef UTILS
#define UTILS

// Input/Output
void drawTitle(const char *);
void clearScreen();
void readString(char *buffer, int size);
void clearInput();
char readOption(const char *valid);

// funzioni per il menu.c
void story();
void playerStats();
void rest();
void missionMenu();
int missionCompleted();
bool isCompleted(DungeonType type);

// funzioni per game.c
bool returnHome(int prize);
int calculateDamage(int baseDamage);
int calculateDiceBonus();
int rollDice();

// funzioni per il dungeon
void printDungeon(Dungeon* dungeon);

#endif
