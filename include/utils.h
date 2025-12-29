/**
 * @file utils.h
 * @brief Funzioni di utilità per I/O, grafica e logica di gioco.
 */

#ifndef UTILS
#define UTILS

#include "types.h"

/**
 * @brief Disegna un titolo incorniciato con bordi ASCII
 * @param string Il testo del titolo da visualizzare
 */
void drawTitle(const char* string);

/**
 * @brief Pulisce lo schermo del terminale
 */
void clearScreen();

/**
 * @brief Legge una stringa dall'input standard con validazione
 * @param buffer Buffer dove salvare la stringa letta
 * @param size Dimensione massima del buffer
 */
void readString(char* buffer, int size);

/**
 * @brief Svuota il buffer di input
 */
void clearInput();

/**
 * @brief Legge un singolo carattere dall'input validandolo
 * @param valid Stringa contenente i caratteri validi
 * @return Il carattere valido letto
 */
char readOption(const char* valid);

/**
 * @brief Verifica se l'utente inserisce il codice Konami
 * @return true se il codice è corretto, false altrimenti
 */
bool konamiCode();

/**
 * @brief Mostra la storia introduttiva del gioco
 */
void story();

/**
 * @brief Visualizza le statistiche del giocatore
 * @param player Puntatore al giocatore
 */
void playerStats(Player* player);

/**
 * @brief Ripristina i punti vita del giocatore al massimo
 * @param player Puntatore al giocatore
 */
void rest(Player* player);

/**
 * @brief Visualizza il menu delle opzioni durante una missione
 */
void missionMenu();

/**
 * @brief Conta quante missioni sono state completate
 * @param player Puntatore al giocatore
 * @return Numero di missioni completate
 */
int missionCompleted(Player* player);

/**
 * @brief Verifica se una specifica missione è completata
 * @param player Puntatore al giocatore
 * @param type Tipo di dungeon da verificare
 * @return true se completata, false altrimenti
 */
bool isCompleted(Player* player, DungeonType type);

/**
 * @brief Gestisce il ritorno al villaggio con pagamento
 * @param player Puntatore al giocatore
 * @param prize Costo del ritorno
 * @return true se il pagamento è andato a buon fine, false altrimenti
 */
bool returnHome(Player* player, int prize);

/**
 * @brief Calcola il danno effettivo considerando l'armatura
 * @param player Puntatore al giocatore
 * @param baseDamage Danno base da calcolare
 * @return Danno effettivo dopo la riduzione
 */
int calculateDamage(Player* player, int baseDamage);

/**
 * @brief Calcola il bonus al dado in base alle armi possedute
 * @param player Puntatore al giocatore
 * @return Bonus da aggiungere al tiro del dado
 */
int calculateDiceBonus(Player* player);

/**
 * @brief Simula il lancio di un dado a 6 facce
 * @return Numero casuale tra 1 e 6
 */
int rollDice();

/**
 * @brief Simula il lancio di una moneta
 * @return HEAD o TAIL
 */
CoinFace flipCoin();

/**
 * @brief Stampa il percorso completato nel dungeon
 * @param dungeon Puntatore al dungeon
 */
void printDungeon(Dungeon* dungeon);

/**
 * @brief Calcola ricorsivamente la sequenza di Padovan
 * @param number Indice della sequenza
 * @return Valore della sequenza all'indice specificato
 */
int padovanSequence(int number);

/**
 * @brief Verifica se un numero appartiene alla sequenza di Padovan
 * @param target Numero da verificare
 * @return true se appartiene alla sequenza, false altrimenti
 */
bool isPadovanNumber(int target);

#endif
