#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "utils.h"

void drawTitle(const char *string) {
    int len = strlen(string);
    
    // Bordo superiore
    printf("+");
    for(int i = 0; i < len+2; i++) printf("=");
    printf("+\n");
    
    // Titolo
    printf("| %s |\n", string);
    
    // Bordo inferiore
    printf("+");
    for(int i = 0; i < len+2; i++) printf("=");
    printf("+\n");
}

void clearScreen() {
    system("clear");
}

void clearInput() {
    int ch; // int per rilevare l'EOF (-1)
    while((ch = getchar()) != EOF && ch != '\n');
}

void readString(char *buffer, int size) {
    while (1) {
        if (fgets(buffer, size, stdin) == NULL) {
            puts("Errore di lettura, riprova");
            clearInput();
            continue;
        }

        char *newline = strchr(buffer, '\n');
        if (newline) {
            *newline = '\0';
        } else {
            printf("La stringa è troppo lunga, riprova: ");
            clearInput();
            continue;
        }

        if (buffer[0] == '\0') {
            printf("La stringa non può essere vuota, riprova: ");
            continue;
        }

        break;
    }
}

bool konamiCode() {
    const char konamiSequence[] = {'w','s','s','a','d','a','d','b','a',' '};
    char code;
    for(int i = 0; i < sizeof(konamiSequence); i++) {
        printf("Seleziona una delle opzioni [1-3]: ");
        code = readOption("123wsadb ");
        if(code != konamiSequence[i]) {
            return false;
        }
    }

    return true;
}

void story() {
    printf("Il mondo è in rovina sotto l’assedio dell’armata oscura.\n"
        "Tu, semplice abitante scelto dal villaggio, devi fermare \n"
        "il Signore Oscuro e riportare la luce, affrontando terre \n"
        "devastate e nemici implacabili.\n");
}

int missionCompleted(Player* HERO) {
    int completed = 0;
    for (int i = 0; i < QUESTS; i++) {
        completed += HERO->missionComplete[i];
    }

    return completed;
}

void playerStats(Player *player) {
    printf("\n%s | HP: %d | MONETE: %d | MISSIONI COMPLETATE %d/3\n\n", 
        player->name, player->hp, player->coins, missionCompleted(player));
}

void rest(Player *player) {
    clearScreen();
    if(player->hp < MAX_HP) player->hp = MAX_HP;
    printf("Dopo un riposo accanto a un falo l'eroe %s è tornato in piene forze\n", player->name);
    puts("I punti vita sono stati riprestinati");
    printf("Premi un tasto per proseguire...");
    clearInput(); 
}

void missionMenu() {
    puts("1. Esplora la stanza del Dungeon");
    puts("2. Negozio");
    puts("3. Inventario");
    puts("4. Torna al Villaggio ( Paga 50 Monete )");
    printf("Seleziona una delle opzioni del menu [1-4]: ");
}

char readOption(const char *valid) {
    char ch;                    
    while((ch = getchar()) && !strchr(valid, ch)) {
        clearInput();
        printf("Opzione non valida riprova: ");
    }
    clearInput();
    return (char)ch;
}

bool returnHome(Player *player, int prize) {
    bool home = false;
    if(player->coins >= prize) {
        puts("Hai pagato 50 monete...");
        player->coins -= prize;
        home = true;
    }
    else 
        puts("Non hai abbastanza soldi...");        
    clearInput(); 
    return home;
}

// calcola il danno effettivo considerando l'armatura
int calculateDamage(Player *player, int baseDamage) {
    return baseDamage + (player->inventory.hasArmor ? ARMOR_REDUCEDMG : 0);
}

// calcola il bonus al dado se si possiede una delle due spade
int calculateDiceBonus(Player *player) {
    if (player->inventory.hasHeroSword)
        return 2;

    if (player->inventory.hasDmgBuff)
        return 1;

    return 0;
}

int rollDice() {
    return (rand() % 6 + 1);
}

CoinFace flipCoin() {
    return rand() % 2;
}

bool isCompleted(Player *player, DungeonType type) {
    return (player->missionComplete[type]) ? true : false;
}

void printDungeon(Dungeon* dungeon) {
    puts("\n========== PERCORSO COMPLETATO ==========");
    Room* current = dungeon->start;
    while (current != NULL) {
        printf("Stanza %d: ", current->roomNumber + 1);
        if (current->type == TRAP) {
            printf("[TRAPPOLA] %s\n", current->trap.name);
        } else if (current->type == COMBAT) {
            printf("[COMBATTIMENTO] %s\n", current->monster.name);
        } else {
            printf("[VUOTA]\n");
        }
        current = current->nextRoom;
    }
    puts("==========================================\n");
}

int padovanSequence(int number) {
    if(number <= 2)
        return 1;
    
    return padovanSequence(number-2) + padovanSequence(number-3);
}

static bool checkPadovan(int index, int target) {
    int currentValue = padovanSequence(index);
    
    if (currentValue == target)
        return true;
    
    if (currentValue > target)
        return false;
    
    return checkPadovan(index + 1, target);
}

bool isPadovanNumber(int target) {
    if (target < 1) return false;
    if (target == 1) return true;  
    
    return checkPadovan(0, target);
}