#ifndef SAVE
#define SAVE

#include "types.h"

char* currentDateTime(char date[DATESIZE]);
void addSave();
void loadSave(int index);
void showSaves();
void removeSave(int index);

#endif