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
bool konamiCode();
void story();
void playerStats(Player *player);
void rest(Player *player);
void missionMenu();
int missionCompleted(Player *player);
bool isCompleted(Player *player, DungeonType type);

// funzioni per game.c
bool returnHome(Player *player, int prize);
int calculateDamage(Player *player, int baseDamage);
int calculateDiceBonus(Player *player);
int rollDice();
CoinFace flipCoin();

// funzioni per il dungeon
void printDungeon(Dungeon* dungeon);
int padovanSequence(int number);
bool isPadovanNumber(int target);

#endif
