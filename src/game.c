#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "game.h"
#include "utils.h"
#include "menu.h"
#include "dungeon.h"

Player HERO;
GameSaves SAVES;

void initGame() {
    HERO.hp = MAX_HP;
    HERO.coins = 0;
    HERO.potions = 0;
    HERO.hasDmgBuff = false;
    HERO.hasArmor = false;
    HERO.hasCastleKey = false;
    HERO.hasHeroSword = false;
    HERO.isAlive = true;
    HERO.konamiCode = false;
    for (int i = 0; i < QUESTS; i++) {
        HERO.missionComplete[i] = false;
    }
}

void initGameSaves() {
    SAVES.start = NULL;
    SAVES.end = NULL;
}

void freeGameSaves() {

}

static void handleRoomEvent(Room *currentRoom) {
    if (currentRoom->type == TRAP) {
        int trapDamage = calculateDamage(currentRoom->trap.dmg);
        printf("Sei caduto nella trappola %s\n", currentRoom->trap.name);
        printf("Hai subito %d danni\n", trapDamage);
        HERO.hp -= trapDamage;
        if (HERO.hp <= 0)
            gameOver();
    }
    else if (currentRoom->type == COMBAT) {
        combat(&currentRoom->monster);
    }
    else
        puts("Sei entrato in una stanza vuota");
}

void swampDungeon() {
    Dungeon *dungeon = (Dungeon *)malloc(sizeof(Dungeon));
    if (!dungeon) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    Room *currentRoom = NULL;
    char choice;
    int orcGeneralKilled = 0;
    
    dungeon->mission = SWAMP;
    dungeon = generateDungeon(dungeon);

    while (HERO.isAlive)
    {
        clearScreen();
        drawTitle("Palude Putrescente");
        printf("Obiettivo : Eliminare %d Generali Orco\n", SWAMP_ORC);
        printf("Stato di avanzamento : Generale Orco %d/3\n", orcGeneralKilled);
        printf("Stanza numero %d\n", dungeon->roomCount + 1);
        playerStats();
        missionMenu();
        choice = readOption("1234");

        switch (choice)
        {
        case '1':
            clearScreen();
            currentRoom = addRoom(dungeon);

            // Forza boss nelle ultime 3 stanze se necessario
            if (orcGeneralKilled < SWAMP_ORC && dungeon->roomCount > DUNGEON_ROOMS - SWAMP_ORC) {
                currentRoom->type = COMBAT;
                currentRoom->monster = swampMonsters[4];
            }

            handleRoomEvent(currentRoom);

            if (HERO.hp > 0)
            {
                if (currentRoom->type == COMBAT && strcmp(currentRoom->monster.name, "Generale Orco") == 0)
                    orcGeneralKilled++;
            }

            clearInput();
            if (orcGeneralKilled == SWAMP_ORC) {
                clearScreen();
                puts("Hai completato il dungeon!");
                HERO.missionComplete[SWAMP] = true;
                printDungeon(dungeon);
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
    // in caso di morte
    freeDungeon(dungeon);
}

void mansionDungeon() {
    Dungeon *dungeon = (Dungeon *)malloc(sizeof(Dungeon));
    if (!dungeon) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    Room *currentRoom = NULL;
    char choice;
    bool vampireKilled = false;
    
    dungeon->mission = MANSION;
    dungeon = generateDungeon(dungeon);

    while (HERO.isAlive) {
        clearScreen();
        drawTitle("Magione Infestata");
        puts("Obiettivo : Recupera la chiave del Castello del Signore Oscuro, e sconfiggi un Vampiro Superiore.");
        printf("Stato di avanzamento : Chiave %d/1 | Vampiro Superiore %d/1\n", HERO.hasCastleKey, vampireKilled);
        printf("Stanza numero %d\n", dungeon->roomCount + 1);
        playerStats();
        missionMenu();
        choice = readOption("1234");

        switch (choice) {
        case '1':
            clearScreen();
            currentRoom = addRoom(dungeon);

            // Forza la chiave e il vampiro superiore nelle ultime 2 stanze se necessario
            if (!vampireKilled && dungeon->roomCount > DUNGEON_ROOMS - 2) {
                currentRoom->type = COMBAT;
                currentRoom->monster = mansionMonsters[3];
            }
            else if (!HERO.hasCastleKey && dungeon->roomCount > DUNGEON_ROOMS - 2) {
                currentRoom->type = COMBAT;
                currentRoom->monster = mansionMonsters[4];
            }

            handleRoomEvent(currentRoom);

            if (HERO.hp > 0) {
                if (currentRoom->type == COMBAT) {
                    if (strcmp(currentRoom->monster.name, "Vampiro Superiore") == 0)
                        vampireKilled = true;
                    if (strcmp(currentRoom->monster.name, "Demone Custode") == 0)
                        HERO.hasCastleKey = true;
                }
            }

            clearInput();
            if (vampireKilled && HERO.hasCastleKey) {
                clearScreen();
                puts("Hai completato il dungeon!");
                HERO.missionComplete[MANSION] = true;
                printDungeon(dungeon);
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
    // in caso di morte
    freeDungeon(dungeon);
}

void caveDungeon() {
    Dungeon *dungeon = (Dungeon *)malloc(sizeof(Dungeon));
    if (!dungeon) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    Room *currentRoom = NULL;
    char choice;
    bool dragonKilled = false;
    
    dungeon->mission = CAVE;
    dungeon = generateDungeon(dungeon);

    while (HERO.isAlive) {
        clearScreen();
        drawTitle("Grotta di Cristallo");
        puts("Obiettivo : Recupera la spada del’Eroe.");
        printf("Stanza numero %d\n", dungeon->roomCount + 1);
        playerStats();
        missionMenu();
        choice = readOption("1234");

        switch (choice) {
        case '1':
            clearScreen();
            currentRoom = addRoom(dungeon);

            // Forza il drago antico
            if (!dragonKilled && dungeon->roomCount > DUNGEON_ROOMS - 1) {
                currentRoom->type = COMBAT;
                currentRoom->monster = caveMonster;
            }

            handleRoomEvent(currentRoom);
            if (currentRoom->type == TRAP && currentRoom->trap.coin > 0) {
                printf("Hai ottenuto %d monete\n", currentRoom->trap.coin);
            }

            if (HERO.hp > 0) {
                if (currentRoom->type == COMBAT) {
                    dragonKilled = true;
                    HERO.hasHeroSword = true;
                }
            }

            clearInput();
            if (dragonKilled) {
                clearScreen();
                puts("Hai completato il dungeon!");
                HERO.missionComplete[CAVE] = true;
                printDungeon(dungeon);
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

void bossFight() {
    BossRoom *room = (BossRoom *)malloc(sizeof(BossRoom));
    if (!room) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    Move move;
    char choice;
    
    room->round = 0;
    room->win = 0;
    room->lose = 0;

    while (HERO.isAlive) {
        clearScreen();
        drawTitle("Signore Oscuro");
        puts("Obiettivo : Sconfiggi il male che domina le terre del nostro mondo");
        printf("Scontro Finale | Round %d su %d| Eroe %d - Signore Oscuro %d.\n",
               room->round, BOSS_FIGHT_ROUNDS, room->win, room->lose);
        printf("\nMosse disponibili:\n1. Scudo\n2. Magia\n3. Spada\n");
        printf("Seleziona una delle opzioni del menu [1-3]: ");
        choice = readOption("123");
        switch (choice) {
        case '1':
            move = SHIELD;
            break;
        case '2':
            move = MAGIC;
            break;
        case '3':
            move = SWORD;
            break;
        default:
            break;
        }
        finalcombat(move, room);
    }
    free(room);
}

void gameOver() {
    printf("\nSei stato sconfitto!\n");
    HERO.isAlive = false;
}

void combat(Monster *monster) {
    printf("Hai incontrato %s\n", monster->name);
    bool isDragon = (strcmp(monster->name, "Drago Antico") == 0);

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

            if (isDragon) {
                int randomNum = rand() % 500 + 1;
                printf("\n=== IL DRAGO TI SFIDA ===\n");
                printf("Il numero %d appartiene alla sequenza di Padovan?\n", randomNum);
                printf("Rispondi [S/N]: ");

                char choice = readOption("SN");

                if (choice == 'S' && isPadovanNumber(randomNum)) {
                    printf("\nRisposta CORRETTA! Il Drago è impressionato dalla tua saggezza.\n");
                    printf("Il danno viene annullato!\n\n");
                    monsterDamage = 0;
                }
                else
                    printf("\nRisposta ERRATA! Il Drago infligge il colpo con furia!\n");
            }

            if (monsterDamage > 0) {
                printf("Hai subito %d danni! (%d < %d)\n", monsterDamage, dice, monster->fatalBlow);
                HERO.hp -= monsterDamage;
            }

            if (HERO.hp <= 0) {
                gameOver();
                break;
            }
        }
    }
}

void finalcombat(Move playerMove, BossRoom *room) {
    Move bossMove = rand() % 3;

    printf("Il Signore Oscuro nel frattempo ha scelto...");
    if (bossMove == SHIELD)
        printf("SCUDO\n\n");
    else if (bossMove == SWORD)
        printf("SPADA\n\n");
    else
        printf("MAGIA\n\n");

    if (playerMove == bossMove) {
        printf("Pareggio! Entrambi avete scelto la stessa mossa.\n");
        clearInput();
        return;
    }
    else if ((playerMove == SHIELD && bossMove == SWORD) ||
             (playerMove == SWORD && bossMove == MAGIC) ||
             (playerMove == MAGIC && bossMove == SHIELD)) {
        // Eroe vince in caso di pareggio
        room->win++;

        if (playerMove == SHIELD && bossMove == SWORD)
            printf("L'eroe para e si difende dalla Spada del Signore Oscuro. L'eroe si aggiudica il Round.\n");
        else if (playerMove == SWORD && bossMove == MAGIC)
            printf("L'eroe taglia la Magia del Signore Oscuro con la sua Spada. L'eroe si aggiudica il Round.\n");
        else if (playerMove == MAGIC && bossMove == SHIELD)
            printf("La Magia dell'eroe supera lo Scudo del Signore Oscuro. L'eroe si aggiudica il Round.\n");
    }
    else {
        // Boss vince
        room->lose++;

        if (bossMove == SHIELD && playerMove == SWORD)
            printf("Il Signore Oscuro para la tua Spada con il suo Scudo. Il Signore Oscuro si aggiudica il Round.\n");
        else if (bossMove == SWORD && playerMove == MAGIC)
            printf("Il Signore Oscuro taglia la tua Magia con la sua Spada. Il Signore Oscuro si aggiudica il Round.\n");
        else if (bossMove == MAGIC && playerMove == SHIELD)
            printf("La Magia del Signore Oscuro supera il tuo Scudo. Il Signore Oscuro si aggiudica il Round.\n");
    }

    room->round++;
    clearInput();

    if (room->win == 3) {
        clearScreen();
        drawTitle("VITTORIA!");
        puts("\n*** HAI SCONFITTO IL SIGNORE OSCURO! ***\n");
        puts("Il male è stato debellato e la luce torna a splendere sul mondo.");
        puts("Sei il salvatore del regno!\n");
        printf("Premi un tasto per tornare al villaggio...");
        clearInput();
        HERO.isAlive = false; // Fine gioco
    }
    else if (room->lose == 3) {
        clearScreen();
        drawTitle("SCONFITTA");
        puts("\n*** IL SIGNORE OSCURO TI HA SCONFITTO! ***\n");
        puts("Le tenebre continuano a dominare il mondo...\n");
        gameOver();
        clearInput();
    }
}