#ifndef UTILS
#define UTILS

void drawTitle(const char *);
void clearScreen();
void readString(char *buffer, int size);
void clearInput();
void story();
void playerStats();
void rest();
void missionMenu();
int missionCompleted();
char readOption(const char *valid);

void returnHome(int prize);
int calculateDamage(int baseDamage);
int calculateDiceBonus();
int rollDice();

#endif
