#ifndef UTILS
#define UTILS

void drawTitle(const char *);
void clearScreen();
void readString(char *buffer, int size);
void clearInput();
void story();
void playerStats(const Player *hero);
int missionCompleted(const Player *hero);
char readOption(const char *valid);

#endif
