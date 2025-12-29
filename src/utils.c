#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "utils.h"

/**
 * @brief Disegna un titolo incorniciato con bordi ASCII
 * @param string Il testo del titolo da visualizzare
 */
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

/**
 * @brief Pulisce lo schermo del terminale
 */
void clearScreen() {
    system("clear");
}

/**
 * @brief Svuota il buffer di input
 */
void clearInput() {
    int ch; // int per rilevare l'EOF (-1)
    while((ch = getchar()) != EOF && ch != '\n');
}

/**
 * @brief Legge una stringa dall'input standard con validazione
 * 
 * Questa funzione legge una stringa dall'input assicurandosi che:
 * - Non superi la dimensione del buffer
 * - Non contenga caratteri di newline residui
 * - Non sia una stringa vuota
 * 
 * In caso di errori, chiede all'utente di reinserire l'input fino a quando
 * non viene fornita una stringa valida.
 * 
 * @param buffer Buffer dove salvare la stringa letta
 * @param size Dimensione massima del buffer
 */
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

/**
 * @brief Verifica se l'utente inserisce il codice Konami
 * 
 * Implementa il Konami Code con la sequenza: w s s a d a d b a spazio.
 * L'utente deve inserire correttamente tutti i caratteri nella sequenza
 * esatta per sbloccare il menu trucchi.
 * 
 * @return true se il codice è corretto, false altrimenti
 */
bool konamiCode() {
    const char konamiSequence[] = {'w','s','s','a','d','a','d','b','a',' '};
    char code;
    
    for(size_t i = 0; i < sizeof(konamiSequence); i++) {
        printf("Seleziona una delle opzioni [1-3]: ");
        code = readOption("123wsadb ");
        
        if(code != konamiSequence[i]) {
            return false;
        }
    }

    return true;
}
/**
 * @brief Mostra la storia introduttiva del gioco
 */void story() {
    printf("Il mondo è in rovina sotto l’assedio dell’armata oscura.\n"
        "Tu, semplice abitante scelto dal villaggio, devi fermare \n"
        "il Signore Oscuro e riportare la luce, affrontando terre \n"
        "devastate e nemici implacabili.\n");
}

/**
 * @brief Conta quante missioni sono state completate
 * @param HERO Puntatore al giocatore
 * @return Numero di missioni completate
 */
int missionCompleted(Player* player) {
    int completed = 0;
    for (int i = 0; i < QUESTS; i++) {
        completed += player->missionComplete[i];
    }

    return completed;
}

/**
 * @brief Visualizza le statistiche del giocatore
 * @param player Puntatore al giocatore
 */
void playerStats(Player *player) {
    printf("\n%s | HP: %d | MONETE: %d | MISSIONI COMPLETATE %d/3\n\n", 
        player->name, player->hp, player->coins, missionCompleted(player));
}

/**
 * @brief Ripristina i punti vita del giocatore al massimo
 * @param player Puntatore al giocatore
 */
void rest(Player *player) {
    clearScreen();
    if(player->hp < MAX_HP) player->hp = MAX_HP;
    printf("Dopo un riposo accanto a un falo l'eroe %s è tornato in piene forze\n", player->name);
    puts("I punti vita sono stati riprestinati");
    printf("Premi un tasto per proseguire...");
    clearInput(); 
}

/**
 * @brief Visualizza il menu delle opzioni durante una missione
 */
void missionMenu() {
    puts("1. Esplora la stanza del Dungeon");
    puts("2. Negozio");
    puts("3. Inventario");
    puts("4. Torna al Villaggio ( Paga 50 Monete )");
    printf("Seleziona una delle opzioni del menu [1-4]: ");
}

/**
 * @brief Legge un singolo carattere dall'input validandolo
 * @param valid Stringa contenente i caratteri validi
 * @return Il carattere valido letto
 */
char readOption(const char *valid) {
    char ch;                    
    while((ch = getchar()) && !strchr(valid, ch)) {
        clearInput();
        printf("Opzione non valida riprova: ");
    }
    clearInput();
    return (char)ch;
}

/**
 * @brief Gestisce il ritorno al villaggio con pagamento
 * @param player Puntatore al giocatore
 * @param prize Costo del ritorno
 * @return true se il pagamento è andato a buon fine, false altrimenti
 */
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

/**
 * @brief Calcola il danno effettivo considerando l'armatura
 * 
 * Modifica il danno base applicando la riduzione fornita dall'armatura.
 * Se il giocatore possiede l'armatura, il danno viene ridotto di ARMOR_REDUCEDMG.
 * 
 * @param player Puntatore al giocatore
 * @param baseDamage Danno base da calcolare
 * @return Danno effettivo dopo la riduzione
 */
int calculateDamage(Player *player, int baseDamage) {
    return baseDamage + (player->inventory.hasArmor ? ARMOR_REDUCEDMG : 0);
}

/**
 * @brief Calcola il bonus al dado in base alla spada posseduta
 * 
 * Determina il modificatore da aggiungere al tiro del dado durante i combattimenti.
 * I bonus sono:
 * - Spada dell'Eroe: +2
 * - Spada Potenziata: +1 
 * 
 * @param player Puntatore al giocatore
 * @return Bonus da aggiungere al tiro del dado
 */
int calculateDiceBonus(Player *player) {
    if (player->inventory.hasHeroSword)
        return 2;

    if (player->inventory.hasDmgBuff)
        return 1;

    return 0;
}

/**
 * @brief Simula il lancio di un dado a 6 facce
 * @return Numero casuale tra 1 e 6
 */
int rollDice() {
    return (rand() % 6 + 1);
}

/**
 * @brief Simula il lancio di una moneta
 * @return HEAD o TAIL
 */
CoinFace flipCoin() {
    return rand() % 2;
}

/**
 * @brief Verifica se una specifica missione è completata
 * @param player Puntatore al giocatore
 * @param type Tipo di dungeon da verificare
 * @return true se completata, false altrimenti
 */
bool isCompleted(Player *player, DungeonType type) {
    return (player->missionComplete[type]) ? true : false;
}

/**
 * @brief Stampa il percorso completato nel dungeon
 * 
 * Visualizza un riepilogo di tutte le stanze attraversate durante l'esplorazione
 * del dungeon, mostrando il numero della stanza e il tipo di evento incontrato
 * (trappola, combattimento o stanza vuota).
 * 
 * @param dungeon Puntatore al dungeon
 */
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

/**
 * @brief Calcola ricorsivamente la sequenza di Padovan
 * 
 * La sequenza di Padovan è una successione matematica simile a Fibonacci,
 * definita dalla relazione di ricorrenza: P(n) = P(n-2) + P(n-3)
 * con valori iniziali P(0) = P(1) = P(2) = 1.
 * 
 * Sequenza: 1, 1, 1, 2, 2, 3, 4, 5, 7, 9, 12, 16, 21, 28...
 * 
 * @param number Indice della sequenza
 * @return Valore della sequenza all'indice specificato
 */
int padovanSequence(int number) {
    if(number <= 2)
        return 1;
    
    return padovanSequence(number-2) + padovanSequence(number-3);
}

/**
 * @brief Funzione ricorsiva per verificare la sequenza di Padovan
 * 
 * Verifica se un numero appartiene alla sequenza di Padovan scorrendola
 * ricorsivamente. Si ferma quando trova il numero cercato o quando
 * supera il target.
 * 
 * @param index Indice corrente della sequenza
 * @param target Numero target da verificare
 * @return true se il target è nella sequenza, false altrimenti
 */
static bool checkPadovan(int index, int target) {
    int currentValue = padovanSequence(index);
    
    if (currentValue == target)
        return true;
    
    if (currentValue > target)
        return false;
    
    return checkPadovan(index + 1, target);
}

/**
 * @brief Verifica se un numero appartiene alla sequenza di Padovan
 * @param target Numero da verificare
 * @return true se appartiene alla sequenza, false altrimenti
 */
bool isPadovanNumber(int target) {
    if (target < 1) return false;
    if (target == 1) return true;  
    
    return checkPadovan(0, target);
}