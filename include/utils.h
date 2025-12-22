#ifndef UTILS
#define UTILS

#include "types.h"

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
CoinFace flipCoin();

// funzioni per il dungeon
void printDungeon(Dungeon* dungeon);
int padovanSequence(int number);
bool isPadovanNumber(int target);

#endif
