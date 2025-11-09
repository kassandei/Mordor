#include "types.h"

// Uso una variabile globale visto che il giocatore è unico e uno solo
// evitando di passare la struttura per ogni funzione come parametro
Player HERO;

Monster swampMonsters[] = {
    {"Cane Selvaggio", 2, 1, 0},      // fatalBlow, dmg, coin
    {"Goblin", 3, 2, 2},
    {"Scheletro", 4, 2, 4},
    {"Orco", 3, 4, 6},
    {"Generale Orco", 6, 3, 12}
};

Trap swampTrap = {
    .name = "Acquitrino Velenoso",
};

