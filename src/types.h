#ifndef RISK_H
#define RISK_H

#include <stdbool.h>
#include <stdint.h>
#include <fileioc.h>
#include <graphx.h>

#define MAP_WIDTH 140
#define MAP_HEIGHT 100

typedef struct {
    uint8_t id;
    uint8_t nTerritories;
    uint8_t* userTerritories;
    uint8_t nContinents;
    uint8_t* continents;
    uint8_t newTroops;
}user_t;

typedef struct {
    uint8_t id;
    char* name;
    user_t* owner;
    uint8_t bonus;
    uint8_t nTerritories;
    uint8_t* territories;
}continent_t;

typedef struct {
    uint8_t id;
    char* name;
    unsigned int x;
    unsigned int y;
    user_t* owner;
    uint8_t continent;
    int nTroops;
    uint8_t nConnections;
    uint8_t* connIndexes;
}territory_t;

typedef struct {
    uint8_t nameLength;
    char* name;
    gfx_sprite_t* map;
    uint8_t nTerritories;
    territory_t** territories;
    uint8_t nContinents;
    continent_t** continents;
    uint8_t nUsers;
    user_t** users;
    unsigned char* palette;
    uint8_t paletteSize;
}game_t;

/*====DICE====*/
/**
 * @brief Performs a single attack between two territories
 * using maximum amount of dice 
 * 
 * @param attacker Pointer to attacking territory
 * @param defender Pointer to defending territory
 * @return true if attacker won, false if attacker lost
 */
bool singleAttack(territory_t* attacker, territory_t* defender);

/**
 * @brief Attacks until \p attacker reaches \p limit troops or defender loses
 * 
 * @param attacker Pointer to attacking territory
 * @param defender Pointer to defending territory
 * @param limit Lowest amount of troops for \p attacker to end with
 * @return true if attacker won, false if limit was reached
 */
bool limitRoll(territory_t* attacker, territory_t* defender, int limit);
/**
 * @brief Attacks until \p attacker reaches 1 troop or defender loses
 * Effectively runs limitRoll with \p limit of 1
 * 
 * @param attacker Pointer to attacking territory
 * @param defender Pointer to defending territory
 * @return true if attacker won, false if limit was reached
 */
bool blitzRoll(territory_t* attacker, territory_t* defender);

/*====GAME====*/

/**
 * @brief Initializes \p game for \p nUsers reading data from \p fileName
 * 
 * @param game Pointer to game to initialize
 * @param fileName Name of appvar to read data from
 * @param nUsers Number of users to create for the game
 */
void initGame(game_t* game, char* fileName, uint8_t nUsers);

/**
 * @brief Randomly assigns all territories to users. \p game must be initialized before randomly assigning territories
 * 
 * @param game Pointer to initiated game
 * 
 */
void randomAssignTerritories(game_t* game);

/**
 * @brief Frees all dynamic memory allocated in \p game 
 * 
 * @param game  Pointer to game to free
 */
void freeGame(game_t* game);

/**
 * @brief Adds troops to a given territory
 * 
 * @param territory Pointer to territory to add troops to
 * @param numTroops Number of troops to be added
 */
void addTroops(territory_t* territory, int numTroops);

/**
 * @brief Removes troops from a given territory
 * 
 * @param territory Pointer to territory to remove troops from
 * @param numTroops Number of troops to be removed
 */
void removeTroops(territory_t* territory, int numTroops);

/**
 * @brief Transfers \p numMove troops from \p from to \p to
 * 
 * @param from Pointer to territory to remove troops from
 * @param to Pointer to territory to add troops to
 * @param numMove Number of troops to transfer
 */
void transferTroops(territory_t* from, territory_t* to, int numMove);

/**
 * @brief Calculates the number of troops a user should gain
 * 
 * @param user Pointer to user to calculate gain
 * 
 * @return Number of troops the user should gain
 */
int gain(user_t* user);

/*====TERRITORY====*/
void initTerritories(territory_t**, uint8_t, ti_var_t);
void freeTerritory(territory_t*);
void freeAllTerritories(territory_t**, uint8_t);
void gainTerritory(user_t*, territory_t*, gfx_sprite_t*, uint8_t*);
void loseTerritory(user_t*, territory_t*);

/*====CONTINENT====*/
void initContinents(continent_t**, uint8_t, ti_var_t);
void freeContinent(continent_t*);
void freeAllContinents(continent_t**, uint8_t);
bool checkContinent(user_t*, continent_t*);
void gainContinent(user_t*, continent_t*);
void loseContinent(user_t*, continent_t*);

/*====USER====*/
void initUsers(user_t**, uint8_t, uint8_t, uint8_t);
void freeUser(user_t*);
void freeAllUsers(user_t**, uint8_t);

/*====TESTING====*/
void dispTerritory(territory_t*, int x, int y);
void dispContinent(continent_t*, int x, int y);
void dispUser(user_t* user, int x, int y);
void runTestFile();

#endif