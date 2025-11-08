#ifndef UTILS
#define UTILS

void drawTitle(const char *);
void clearScreen();
void readString(char *buffer, int size);
void clearInput();
void story();
void playerStats();
int missionCompleted();
char readOption(const char *valid);
void returnHome(int prize);

#endif
