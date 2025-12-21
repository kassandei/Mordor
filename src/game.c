#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "game.h"
#include "utils.h"
#include "menu.h"
#include "dungeon.h"

Player HERO;

void initGame() {
    HERO.hp = MAX_HP;
    HERO.coins = 0;
    HERO.potions = 0;
    HERO.hasDmgBuff = false;
    HERO.hasArmor = false;
    HERO.hasCastleKey = false;
    HERO.hasHeroSword = false;
    HERO.isAlive = true;
    for (int i = 0; i < QUESTS; i++) {
        HERO.missionComplete[i] = false;
    }
}

void swampDungeon() {
    Dungeon *dungeon = (Dungeon*)malloc(sizeof(Dungeon));
    int obj = 0;
    dungeon->mission = SWAMP;
    dungeon->canExit = false;
    dungeon = generateDungeon(dungeon);
    char choice;
    while (HERO.isAlive) {
        clearScreen();
        drawTitle("Palude Putrescente");
        printf("Obiettivo : Eliminare %d Generali Orco\n", SWAMP_ORC);
        printf("Stato di avanzamento : Generale Orco %d/3\n", obj);
        printf("Stanza numero %d\n", dungeon->room->roomNumber + 1);
        playerStats();
        missionMenu();
        choice = readOption("1234");

        switch (choice) {
        case '1':
            clearScreen();

            // Forza boss nelle ultime 3 stanze se necessario
            if (obj < SWAMP_ORC && dungeon->room->roomNumber >= DUNGEON_ROOMS - SWAMP_ORC) {
                dungeon->room->type = COMBAT;
                dungeon->room->monster = swampMonsters[4];
            }

            if (dungeon->room->type == TRAP) {
                int trapDamage = calculateDamage(dungeon->room->trap.dmg);
                printf("Sei caduto nella trappola %s\n", dungeon->room->trap.name);
                printf("Hai subito %d danni\n", trapDamage);
                HERO.hp -= trapDamage;
                if (HERO.hp <= 0)
                    gameOver();
            }
            else {
                combat(&dungeon->room->monster);

                if (strcmp(dungeon->room->monster.name, "Generale Orco") == 0) {
                    obj++;
                }
            }
            dungeon->room = dungeon->room->nextRoom;
            clearInput();
            if (obj == SWAMP_ORC) {
                clearScreen();
                puts("Hai completato il dungeon!");
                HERO.missionComplete[SWAMP] = true;
                clearInput();
                freeDungeon(dungeon);
                return;
            }
            break;
        case '2':
            shopMenu();
            break;
        case '3':
            inventoryMenu();
            break;
        case '4':
            if (returnHome(PRICE_RETURN)) {
                freeDungeon(dungeon);
                return;
            }
            break;
        default:
            break;
        }
    }
    freeDungeon(dungeon);
}

void mansionDungeon() {
    Dungeon *dungeon = (Dungeon*)malloc(sizeof(Dungeon));
    bool obj = false;
    dungeon->mission = MANSION;
    dungeon->canExit = false;
    dungeon = generateDungeon(dungeon);
    char choice;

    while (HERO.isAlive) {
        clearScreen();
        drawTitle("Magione Infestata");
        puts("Obiettivo : Recupera la chiave del Castello del Signore Oscuro, e sconfiggi un Vampiro Superiore.");
        printf("Stato di avanzamento : Chiave %d/1 | Vampiro Superiore %d/1", HERO.hasCastleKey, obj);
        playerStats();
        missionMenu();
        choice = readOption("1234");

        switch (choice)
        {
        case '1':
            clearScreen();

            // Forza la chiave e il vampiro superiore nelle ultime 2 stanze se necessario
            if (!obj && dungeon->room->roomNumber >= DUNGEON_ROOMS - MANSION_MONSTERS) {
                dungeon->room->type = COMBAT;
                dungeon->room->monster = mansionMonsters[3];
            }
            else if (!HERO.hasCastleKey && dungeon->room->roomNumber >= DUNGEON_ROOMS - MANSION_MONSTERS) {
                dungeon->room->type = COMBAT;
                dungeon->room->monster = mansionMonsters[4];
            }

            if (dungeon->room->type == TRAP) {
                int trapDamage = calculateDamage(dungeon->room->trap.dmg);
                printf("Sei caduto nella trappola %s\n", dungeon->room->trap.name);
                printf("Hai subito %d danni\n", trapDamage);
                HERO.hp -= trapDamage;
                if (HERO.hp <= 0)
                    gameOver();
            }
            else {
                combat(&dungeon->room->monster);

                if (strcmp(dungeon->room->monster.name, "Vampiro Superiore") == 0) {
                    obj = true;
                }
                if (strcmp(dungeon->room->monster.name, "Demone Custode") == 0) {
                    HERO.hasCastleKey = true;
                }
            }
            dungeon->room = dungeon->room->nextRoom;
            clearInput();
            if (obj && HERO.hasCastleKey) {
                clearScreen();
                puts("Hai completato il dungeon!");
                HERO.missionComplete[MANSION] = true;
                clearInput();
                freeDungeon(dungeon);
                return;
            }
            break;
        case '2':
            shopMenu();
            break;
        case '3':
            inventoryMenu();
            break;
        case '4':
            if (returnHome(PRICE_RETURN)) {
                freeDungeon(dungeon);
                return;
            }
            break;
        default:
            break;
        }
    freeDungeon(dungeon);
    }
}

void caveDungeon()
{
    char choice;

    while (HERO.isAlive) {
        clearScreen();
        drawTitle("Grotta di Cristallo");
        puts("Obiettivo : Recupera la spada del’Eroe.");
        printf("Stato di avanzamento : spada %d/1", 0);
        playerStats();
        missionMenu();
        choice = readOption("1234");

        switch (choice)
        {
        case '1':
            break;
        case '2':
            shopMenu();
            break;
        case '3':
            inventoryMenu();
            break;
        case '4':
            returnHome(PRICE_RETURN);
            break;
        default:
            break;
        }
    }
}

void gameOver() {
    printf("\nSei stato sconfitto!\n");
    HERO.isAlive = false;
}

void combat(Monster *monster) {
    printf("Hai incontrato %s\n", monster->name);
    while (1) {
        int dice = rollDice() + calculateDiceBonus();

        printf("Premi un tasto per tirare il dado...");
        clearInput();

        printf("Dal tiro del dado è uscito %d\n", dice);
        if (dice >= monster->fatalBlow) {
            HERO.coins += monster->coin;
            printf("Hai battuto il nemico! (%d >= %d)\n", dice, monster->fatalBlow);
            printf("Hai ottenuto %d monete\n", monster->coin);
            printf("Premi un tasto per uscire...");
            break;
        }
        else {
            int monsterDamage = calculateDamage(monster->dmg);
            printf("Hai subito %d danni! (%d < %d)\n", monsterDamage, dice, monster->fatalBlow);
            HERO.hp -= monsterDamage;
            if (HERO.hp <= 0) {
                gameOver();
                break;
            }
        }
    }
}
