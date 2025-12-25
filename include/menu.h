#ifndef MENU
#define MENU

#include "types.h"

void menu(Player *player, GameSaves *saves);
void cheatMenu(Player *player);
void newGame(Player *player, GameSaves *saves);
void loadGame(Player *player, GameSaves *saves, bool fromCheatMenu);
void villageMenu(Player *player, GameSaves *saves);
void dungeonMenu(Player *player);
void shopMenu(Player *player);
void saveGame(Player *player, GameSaves *saves);
void inventoryMenu(Player *player);

#endif