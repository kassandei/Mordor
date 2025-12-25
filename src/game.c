#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "game.h"
#include "utils.h"
#include "menu.h"
#include "dungeon.h"

void initGame(Player *player) {
    player->hp = MAX_HP;
    player->coins = 0;
    player->inventory.potions = 0;
    player->inventory.hasDmgBuff = false;
    player->inventory.hasArmor = false;
    player->inventory.hasCastleKey = false;
    player->inventory.hasHeroSword = false;
    player->isAlive = true;
    player->konamiCode = false;
    for (int i = 0; i < QUESTS; i++) {
        player->missionComplete[i] = false;
    }
}

void initGameSaves(GameSaves *saves) {
    saves->start = NULL;
    saves->end = NULL;
}

void freeGameSaves(GameSaves *saves) {
    while(saves->start != NULL) {
        SaveFile* tmp = saves->start;
        saves->start = saves->start->nextSaveFile;
        free(tmp);
    }
}

static void handleRoomEvent(Player *player, Room *currentRoom) {
    if (currentRoom->type == TRAP) {
        int trapDamage = calculateDamage(player, currentRoom->trap.dmg);
        printf("Sei caduto nella trappola %s\n", currentRoom->trap.name);
        printf("Hai subito %d danni\n", trapDamage);
        player->hp -= trapDamage;
        if (player->hp <= 0)
            gameOver(player);
    }
    else if (currentRoom->type == COMBAT) {
        combat(player, &currentRoom->monster);
    }
    else
        puts("Sei entrato in una stanza vuota");
}

void swampDungeon(Player *player) {
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

    while (player->isAlive)
    {
        clearScreen();
        drawTitle("Palude Putrescente");
        printf("Obiettivo : Eliminare %d Generali Orco\n", SWAMP_ORC);
        printf("Stato di avanzamento : Generale Orco %d/3\n", orcGeneralKilled);
        printf("Stanza numero %d\n", dungeon->roomCount + 1);
        playerStats(player);
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

            handleRoomEvent(player, currentRoom);

            if (player->hp > 0) {
                if (currentRoom->type == COMBAT && strcmp(currentRoom->monster.name, "Generale Orco") == 0)
                    orcGeneralKilled++;
            }

            clearInput();
            if (orcGeneralKilled == SWAMP_ORC) {
                clearScreen();
                puts("Hai completato il dungeon!");
                player->missionComplete[SWAMP] = true;
                printDungeon(dungeon);
                clearInput();
                freeDungeon(dungeon);
                return;
            }
            break;
        case '2':
            shopMenu(player);
            break;
        case '3':
            inventoryMenu(player);
            break;
        case '4':
            if (returnHome(player, PRICE_RETURN)) {
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

void mansionDungeon(Player *player) {
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

    while (player->isAlive) {
        clearScreen();
        drawTitle("Magione Infestata");
        puts("Obiettivo : Recupera la chiave del Castello del Signore Oscuro, e sconfiggi un Vampiro Superiore.");
        printf("Stato di avanzamento : Chiave %d/1 | Vampiro Superiore %d/1\n", player->inventory.hasCastleKey, vampireKilled);
        printf("Stanza numero %d\n", dungeon->roomCount + 1);
        playerStats(player);
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
            else if (!player->inventory.hasCastleKey && dungeon->roomCount > DUNGEON_ROOMS - 2) {
                currentRoom->type = COMBAT;
                currentRoom->monster = mansionMonsters[4];
            }

            handleRoomEvent(player, currentRoom);

            if (player->hp > 0) {
                if (currentRoom->type == COMBAT) {
                    if (strcmp(currentRoom->monster.name, "Vampiro Superiore") == 0)
                        vampireKilled = true;
                    if (strcmp(currentRoom->monster.name, "Demone Custode") == 0)
                        player->inventory.hasCastleKey = true;
                }
            }

            clearInput();
            if (vampireKilled && player->inventory.hasCastleKey) {
                clearScreen();
                puts("Hai completato il dungeon!");
                player->missionComplete[MANSION] = true;
                printDungeon(dungeon);
                clearInput();
                freeDungeon(dungeon);
                return;
            }
            break;
        case '2':
            shopMenu(player);
            break;
        case '3':
            inventoryMenu(player);
            break;
        case '4':
            if (returnHome(player, PRICE_RETURN)) {
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

void caveDungeon(Player *player) {
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

    while (player->isAlive) {
        clearScreen();
        drawTitle("Grotta di Cristallo");
        puts("Obiettivo : Recupera la spada del’Eroe.");
        printf("Stanza numero %d\n", dungeon->roomCount + 1);
        playerStats(player);
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

            handleRoomEvent(player, currentRoom);
            if (currentRoom->type == TRAP && currentRoom->trap.coin > 0) {
                printf("Hai ottenuto %d monete\n", currentRoom->trap.coin);
                player->coins += currentRoom->trap.coin;            }

            if (player->hp > 0) {
                if (currentRoom->type == COMBAT) {
                    dragonKilled = true;
                    player->inventory.hasHeroSword = true;
                }
            }

            clearInput();
            if (dragonKilled) {
                clearScreen();
                puts("Hai completato il dungeon!");
                player->missionComplete[CAVE] = true;
                printDungeon(dungeon);
                clearInput();
                freeDungeon(dungeon);
                return;
            }
            break;
        case '2':
            shopMenu(player);
            break;
        case '3':
            inventoryMenu(player);
            break;
        case '4':
            if (returnHome(player, PRICE_RETURN)) {
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

void bossFight(Player *player) {
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

    while (player->isAlive) {
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
        finalcombat(player, move, room);
    }
    free(room);
}

void gameOver(Player *player) {
    printf("\nSei stato sconfitto!\n");
    player->isAlive = false;
}

void combat(Player *player, Monster *monster) {
    printf("Hai incontrato %s\n", monster->name);
    bool isDragon = (strcmp(monster->name, "Drago Antico") == 0);

    while (1) {
        int dice = rollDice() + calculateDiceBonus(player);

        printf("Premi un tasto per tirare il dado...");
        clearInput();

        printf("Dal tiro del dado è uscito %d\n", dice);
        if (dice >= monster->fatalBlow) {
            player->coins += monster->coin;
            printf("Hai battuto il nemico! (%d >= %d)\n", dice, monster->fatalBlow);
            printf("Hai ottenuto %d monete\n", monster->coin);
            printf("Premi un tasto per uscire...");
            break;
        }
        else {
            int monsterDamage = calculateDamage(player, monster->dmg);

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
                player->hp -= monsterDamage;
            }

            if (player->hp <= 0) {
                gameOver(player);
                break;
            }
        }
    }
}

void finalcombat(Player *player, Move playerMove, BossRoom *room) {
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
        // Eroe vince
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
        player->isAlive = false; // Fine gioco
    }
    else if (room->lose == 3) {
        clearScreen();
        drawTitle("SCONFITTA");
        puts("\n*** IL SIGNORE OSCURO TI HA SCONFITTO! ***\n");
        puts("Le tenebre continuano a dominare il mondo...\n");
        gameOver(player);
        clearInput();
    }
}