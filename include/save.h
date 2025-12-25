#ifndef SAVE
#define SAVE

#include "types.h"

char* currentDateTime(char date[DATESIZE]);
void addSave(Player *player, GameSaves *saves);
void loadSave(Player *player, GameSaves *saves, int index);
void showSaves(GameSaves *saves);
void removeSave(GameSaves *saves, int index);

#endif