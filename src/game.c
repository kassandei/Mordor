#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "game.h"
#include "utils.h"
#include "menu.h"
#include "dungeon.h"

/**
 * @brief Inizializza un nuovo giocatore con valori di default
 * 
 * Imposta lo stato iniziale del giocatore per una nuova partita: punti vita
 * al massimo, inventario vuoto, nessuna missione completata.
 * 
 * @param player Puntatore al giocatore da inizializzare
 */
void initGame(Player *player) {
    // Imposta i valori iniziali del giocatore
    player->hp = MAX_HP;
    player->coins = 0;
    
    player->inventory.potions = 0;
    player->inventory.hasDmgBuff = false;
    player->inventory.hasArmor = false;
    player->inventory.hasCastleKey = false;
    player->inventory.hasHeroSword = false;
    
    player->isAlive = true;
    player->konamiCode = false;
    
    // Tutte le missioni inizialmente non completate
    for (int i = 0; i < QUESTS; i++) {
        player->missionComplete[i] = false;
    }
}

/**
 * @brief Inizializza la struttura della lista dei salvataggi
 * 
 * Imposta i puntatori start ed end a NULL creando una lista vuota
 * 
 * @param saves Puntatore alla struttura da inizializzare
 */
void initGameSaves(GameSaves *saves) {
    saves->start = NULL;
    saves->end = NULL;
}

/**
 * @brief Libera la memoria allocata per la lista dei salvataggi
 * 
 * Attraversa la lista concatenata deallocando ciascun nodo per prevenire
 * memory leak al termine del programma.
 * 
 * @param saves Puntatore alla struttura dei salvataggi
 */
void freeGameSaves(GameSaves *saves) {
    while(saves->start != NULL) {
        SaveFile* tmp = saves->start;
        saves->start = saves->start->nextSaveFile;
        free(tmp);
    }
}

/**
 * @brief Gestisce gli eventi che si verificano in una stanza
 * 
 * Determina il tipo di stanza ed esegue le azioni appropriate: applica
 * il danno delle trappole considerando l'armatura, avvia combattimenti
 * contro mostri o notifica stanze vuote. Verifica anche se il giocatore
 * muore per una trappola.
 * 
 * @param player Puntatore al giocatore
 * @param currentRoom Puntatore alla stanza corrente
 */
static void handleRoomEvent(Player *player, Room *currentRoom) {
    if (currentRoom->type == TRAP) {
        // Gestisce una trappola: calcola il danno considerando l'armatura
        int trapDamage = calculateDamage(player, currentRoom->trap.dmg);
        printf("Sei caduto nella trappola %s\n", currentRoom->trap.name);
        printf("Hai subito %d danni\n", trapDamage);
        player->hp -= trapDamage;
        
        // Verifica se il giocatore è morto per la trappola
        if (player->hp <= 0)
            gameOver(player);
    }
    else if (currentRoom->type == COMBAT) {
        combat(player, &currentRoom->monster);
    }
    else
        puts("Sei entrato in una stanza vuota");
}

/**
 * @brief Gestisce il dungeon della Palude Putrescente
 * 
 * Implementazione del primo dungeon con l'obiettivo di sconfiggere 3 Generali Orco.
 * Il sistema forza la comparsa dei boss rimanenti nelle ultime stanze se il
 * giocatore si avvicina alla fine senza aver completato l'obiettivo, garantendo
 * che la missione sia sempre completabile.
 * 
 * @param player Puntatore al giocatore
 */
void swampDungeon(Player *player) {
    Dungeon *dungeon = (Dungeon *)malloc(sizeof(Dungeon));
    if (!dungeon) {
        fprintf(stderr, "Errore: memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
    Room *currentRoom = NULL;
    char choice;
    int orcGeneralKilled = 0;  // Contatore per i boss uccisi
    
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
            // Genera e aggiunge una nuova stanza al dungeon
            currentRoom = addRoom(dungeon);

            // Forza la comparsa del boss nelle ultime stanze se necessario
            if (orcGeneralKilled < SWAMP_ORC && dungeon->roomCount > DUNGEON_ROOMS - SWAMP_ORC) {
                currentRoom->type = COMBAT;
                currentRoom->monster = swampMonsters[4];  // Generale Orco
            }

            // Gestisce l'evento della stanza (trappola, combattimento o vuota)
            handleRoomEvent(player, currentRoom);

            // Se il giocatore è sopravvissuto e ha ucciso un Generale Orco, incrementa il contatore
            if (player->hp > 0) {
                if (currentRoom->type == COMBAT && strcmp(currentRoom->monster.name, "Generale Orco") == 0)
                    orcGeneralKilled++;
            }

            clearInput();
            
            // Verifica se la missione è completata
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

/**
 * @brief Gestisce il dungeon della Magione Infestata
 * 
 * Secondo dungeon con obiettivi multipli: recuperare la chiave del Castello
 * del Signore Oscuro sconfiggendo il Demone Custode e uccidere un Vampiro
 * Superiore. Il sistema forza gli obiettivi nelle ultime stanze se non completati.
 * 
 * @param player Puntatore al giocatore
 */
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

            // Forza gli obiettivi chiave nelle ultime stanze
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

/**
 * @brief Gestisce il dungeon della Grotta di Cristallo
 * 
 * Terzo dungeon con obiettivo di recuperare la Spada dell'Eroe sconfiggendo
 * il Drago Antico. A differenza degli altri, presenta stanze vuote e alcune
 * trappole possono dare ricompense in monete come il Forziere Misterioso.
 * 
 * @param player Puntatore al giocatore
 */
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

            // Forza l'apparizione del drago nell'ultima stanza
            if (!dragonKilled && dungeon->roomCount > DUNGEON_ROOMS - 1) {
                currentRoom->type = COMBAT;
                currentRoom->monster = caveMonster;
            }

            handleRoomEvent(player, currentRoom);
            
            // Gestisce il guadagno di monete dalle trappole
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

/**
 * @brief Gestisce lo scontro finale contro il Signore Oscuro
 * 
 * Combattimento finale strutturato in round stile "sasso-carta-forbici"
 * con tre mosse: Scudo, Magia e Spada. Al meglio di 5 round, il primo
 * a vincere 3 round determina l'esito del gioco.
 * 
 * @param player Puntatore al giocatore
 */
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

/**
 * @brief Gestisce la sconfitta del giocatore
 * 
 * Visualizza il messaggio di sconfitta e imposta il flag isAlive a false,
 * terminando la partita corrente.
 * 
 * @param player Puntatore al giocatore
 */
void gameOver(Player *player) {
    printf("\nSei stato sconfitto!\n");
    player->isAlive = false;
}

/**
 * @brief Gestisce un combattimento standard contro un mostro
 * 
 * Sistema di combattimento basato sul lancio di dadi. Il giocatore continua
 * a tirare finché non sconfigge il mostro o viene sconfitto. Se il risultato
 * è maggiore o uguale al fatalBlow del mostro, vince e ottiene monete, altrimenti
 * subisce danno. Il Drago Antico pone una domanda sulla sequenza di Padovan
 * che può annullare il danno se il giocatore risopnde in modo corretto.
 * 
 * @param player Puntatore al giocatore
 * @param monster Puntatore al mostro da affrontare
 */
void combat(Player *player, Monster *monster) {
    printf("Hai incontrato %s\n", monster->name);
    bool isDragon = (strcmp(monster->name, "Drago Antico") == 0);
    bool isGeneralOrco = (strcmp(monster->name, "Generale Orco") == 0);
    
    // Il Generale Orco ha colpo fatale 6, ma con Spada dell'Eroe diventa 5
    int effectiveFatalBlow = monster->fatalBlow;
    if (isGeneralOrco && player->inventory.hasHeroSword) {
        effectiveFatalBlow = 5;
    }

    while (1) {
        // Tira il dado e aggiungi i bonus da armi/potenziamenti
        int dice = rollDice() + calculateDiceBonus(player);

        printf("Premi un tasto per tirare il dado...");
        clearInput();

        printf("Dal tiro del dado è uscito %d\n", dice);
        
        // Verifica se il tiro è sufficiente per uccidere il mostro
        if (dice >= effectiveFatalBlow) {
            // Vittoria! Ottieni le monete del mostro
            player->coins += monster->coin;
            printf("Hai battuto il nemico! (%d >= %d)\n", dice, effectiveFatalBlow);
            printf("Hai ottenuto %d monete\n", monster->coin);
            printf("Premi un tasto per uscire...");
            break;
        }
        else {
            // Tiro insufficiente: il mostro contrattacca
            int monsterDamage = calculateDamage(player, monster->dmg);

            // Sfida speciale del drago: domanda sulla sequenza di Padovan
            if (isDragon) {
                // Genera un numero casuale tra 1 e 500
                int randomNum = rand() % 500 + 1;
                printf("\n=== IL DRAGO TI SFIDA ===\n");
                printf("Il numero %d appartiene alla sequenza di Padovan?\n", randomNum);
                printf("Rispondi [S/N]: ");

                char choice = readOption("SN");

                // Verifica se la risposta è corretta
                if (choice == 'S' && isPadovanNumber(randomNum)) {
                    printf("\nRisposta CORRETTA! Il Drago è impressionato dalla tua saggezza.\n");
                    printf("Il danno viene annullato!\n\n");
                    monsterDamage = 0;  // Nessun danno per risposta corretta
                }
                else
                    printf("\nRisposta ERRATA! Il Drago infligge il colpo con furia!\n");
            }

            // Applica il danno al giocatore
            if (monsterDamage > 0) {
                printf("Hai subito %d danni! (%d < %d)\n", monsterDamage, dice, effectiveFatalBlow);
                player->hp -= monsterDamage;
            }

            // Verifica se il giocatore è morto
            if (player->hp <= 0) {
                gameOver(player);
                break;
            }
        }
    }
}

/**
 * @brief Gestisce un singolo round del combattimento finale
 * 
 * Implementa la logica del round.
 * In caso di pareggio il round non conta. Il combattimento termina quando
 * uno dei due raggiunge 3 vittorie, determinando l'esito del gioco.
 * 
 * @param player Puntatore al giocatore
 * @param playerMove Mossa scelta dal giocatore
 * @param room Puntatore alla stanza del boss
 */
void finalcombat(Player *player, Move playerMove, BossRoom *room) {
    // Il boss sceglie casualmente una delle tre mosse
    Move bossMove = rand() % 3;

    printf("Il Signore Oscuro nel frattempo ha scelto...");
    if (bossMove == SHIELD)
        printf("SCUDO\n\n");
    else if (bossMove == SWORD)
        printf("SPADA\n\n");
    else
        printf("MAGIA\n\n");

    // SCUDO batte SPADA, SPADA batte MAGIA, MAGIA batte SCUDO
    if (playerMove == bossMove) {
        printf("Pareggio! Entrambi avete scelto la stessa mossa.\n");
        clearInput();
        return;
    }
    else if ((playerMove == SHIELD && bossMove == SWORD) ||
             (playerMove == SWORD && bossMove == MAGIC) ||
             (playerMove == MAGIC && bossMove == SHIELD)) {
        // Il giocatore vince il round
        room->win++;

        if (playerMove == SHIELD && bossMove == SWORD)
            printf("L'eroe para e si difende dalla Spada del Signore Oscuro. L'eroe si aggiudica il Round.\n");
        else if (playerMove == SWORD && bossMove == MAGIC)
            printf("L'eroe taglia la Magia del Signore Oscuro con la sua Spada. L'eroe si aggiudica il Round.\n");
        else if (playerMove == MAGIC && bossMove == SHIELD)
            printf("La Magia dell'eroe supera lo Scudo del Signore Oscuro. L'eroe si aggiudica il Round.\n");
    }
    else {
        // Il boss vince il round
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
        // Vittoria finale: il giocatore ha vinto 3 round
        clearScreen();
        drawTitle("VITTORIA!");
        puts("\n*** HAI SCONFITTO IL SIGNORE OSCURO! ***\n");
        puts("Il male è stato debellato e la luce torna a splendere sul mondo.");
        puts("Sei il salvatore del regno!\n");
        printf("Premi un tasto per tornare al villaggio...");
        clearInput();
        player->isAlive = false; // Termina il gioco con vittoria
    }
    else if (room->lose == 3) {
        // Sconfitta finale: il boss ha vinto 3 round
        clearScreen();
        drawTitle("SCONFITTA");
        puts("\n*** IL SIGNORE OSCURO TI HA SCONFITTO! ***\n");
        puts("Le tenebre continuano a dominare il mondo...\n");
        gameOver(player);
        clearInput();
    }
}