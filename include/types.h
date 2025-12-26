/**
 * @file types.h
 * @brief Definizioni di tipi, strutture ed enumerazioni del gioco.
 */

#ifndef TYPE
#define TYPE

#include <stdbool.h>

#define NAMESIZE 30
#define DATESIZE 30
#define QUESTS 3
#define MAX_HP 20
#define PRICE_RETURN 50
#define POTION_PRICE 4
#define DMGBUFF_PRICE 5
#define ARMOR_PRICE 10
#define DMGBUFF 1
#define ARMOR_REDUCEDMG -1
#define BOSS_FIGHT_ROUNDS 5

/**
 * @brief Tipo di stanza nel dungeon
 */
typedef enum {
    EMPTY,              
    TRAP,
    COMBAT
} RoomType;

/**
 * @brief Mosse disponibili nel combattimento finale
 */
typedef enum {
    SHIELD,
    MAGIC,
    SWORD
} Move;

/**
 * @brief Facce della moneta per il lancio casuale
 */
typedef enum {
    HEAD, 
    TAIL
} CoinFace;

/**
 * @brief Tipi di dungeon disponibili nel gioco
 */
typedef enum {
    SWAMP,
    MANSION,
    CAVE
} DungeonType;

/**
 * @brief Inventario del giocatore contenente oggetti e potenziamenti
 */
typedef struct {
    int potions;            /**< Numero di pozioni possedute */
    bool hasDmgBuff;        /**< Potenziamento danni attivo */
    bool hasArmor;          /**< Armatura equipaggiata */
    bool hasCastleKey;      /**< Chiave del castello ottenuta */
    bool hasHeroSword;      /**< Spada dell'eroe ottenuta */
} Inventory;

/**
 * @brief Struttura principale del giocatore
 */
typedef struct Player {
    char name[NAMESIZE];           /**< Nome del giocatore */
    int hp;                        /**< Punti vita correnti */
    int coins;                     /**< Monete possedute */
    Inventory inventory;           /**< Inventario del giocatore */
    bool missionComplete[QUESTS];  /**< Stato completamento missioni */
    bool isAlive;                  /**< Flag di vita */
    bool konamiCode;               /**< Cheat code Konami attivato */
} Player;

/**
 * @brief Struttura che rappresenta un mostro
 */
typedef struct {
    char* name;        /**< Nome del mostro */
    int fatalBlow;     /**< Valore dado richiesto per ucciderlo */
    int dmg;           /**< Danno inflitto al giocatore */
    int coin;          /**< Monete ottenute sconfiggendolo */
} Monster;

/**
 * @brief Struttura che rappresenta una trappola
 */
typedef struct {
    char* name;    /**< Nome della trappola */
    int dmg;       /**< Danno inflitto */
    int coin;      /**< Monete date o tolte */
} Trap;

/**
 * @brief Nodo della lista concatenata delle stanze
 */
typedef struct Room {
    int roomNumber;           /**< Numero progressivo della stanza */
    RoomType type;            /**< Tipo di stanza */
    Monster monster;          /**< Mostro presente (se COMBAT) */
    Trap trap;                /**< Trappola presente (se TRAP) */
    struct Room* nextRoom;    /**< Puntatore alla prossima stanza */
} Room;

/**
 * @brief Struttura principale del dungeon
 */
typedef struct {
    Room* start;           /**< Prima stanza della lista */
    Room* end;             /**< Ultima stanza della lista */
    DungeonType mission;   /**< Tipo di dungeon corrente */
    int roomCount;         /**< Numero di stanze generate */
} Dungeon;

/**
 * @brief Stato del combattimento contro il boss finale
 */
typedef struct {
    int round;    /**< Round corrente */
    int win;      /**< Round vinti dal giocatore */
    int lose;     /**< Round vinti dal boss */
} BossRoom;

/**
 * @brief Nodo della lista concatenata dei salvataggi
 */
typedef struct SaveFile {
    char date[DATESIZE];              /**< Data del salvataggio */
    int number;                       /**< Numero progressivo del save */
    Player player;                    /**< Dati del giocatore salvati */
    struct SaveFile* nextSaveFile;    /**< Puntatore al prossimo save */
} SaveFile;

/**
 * @brief Lista concatenata dei file di salvataggio
 */
typedef struct {
    SaveFile* start;    /**< Primo salvataggio della lista */
    SaveFile* end;      /**< Ultimo salvataggio della lista */
} GameSaves;

#endif