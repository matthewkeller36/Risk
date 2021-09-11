#ifndef RISK_H
#define RISK_H

#include <stdbool.h>
#include <stdint.h>
#include <fileioc.h>
#include <graphx.h>

#define MAP_WIDTH 140
#define MAP_HEIGHT 100
#define MAP_X_OFFSET 0
#define MAP_Y_OFFSET 0
#define MINI_TEXT_HEIGHT 5

typedef struct user{
    uint8_t id;
    uint8_t nTerritories;
    uint8_t* userTerritories;
    uint8_t newTroops;
}user_t;

typedef struct continent{
    uint8_t id;
    char* name;
    user_t* owner;
    uint8_t bonus;
    uint8_t nTerritories;
    uint8_t* territories;
}continent_t;

typedef struct territory{
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

typedef struct game{
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
 * using maximum amount of dice .
 * 
 * @param attacker Pointer to attacking territory.
 * @param defender Pointer to defending territory.
 * @return true if attacker won, false if attacker lost.
 */
bool singleAttack(territory_t* attacker, territory_t* defender);

/**
 * @brief Attacks until \p attacker reaches \p limit troops or defender loses.
 * 
 * @param attacker Pointer to attacking territory.
 * @param defender Pointer to defending territory.
 * @param limit Lowest amount of troops for \p attacker to end with.
 * @return true if attacker won, false if limit was reached.
 */
bool limitRoll(territory_t* attacker, territory_t* defender, int limit);
/**
 * @brief Attacks until \p attacker reaches 1 troop or defender loses.
 * Effectively runs limitRoll with \p limit of 1.
 * 
 * @param attacker Pointer to attacking territory.
 * @param defender Pointer to defending territory.
 * @return true if attacker won, false if limit was reached.
 */
bool blitzRoll(territory_t* attacker, territory_t* defender);

/*====GAME====*/

/**
 * @brief Initializes \p game for \p nUsers reading data from \p fileName
 * 
 * @param game Pointer to game to initialize.
 * @param fileName Name of appvar to read data from.
 * @param nUsers Number of users to create for the game.
 */
void initGame(game_t* game, char* fileName, uint8_t nUsers);

/**
 * @brief Randomly assigns all territories to users. \p game must be initialized before randomly assigning territories.
 * 
 * @param game Pointer to initiated game.
 * 
 */
void randomAssignTerritories(game_t* game);

/**
 * @brief Frees all dynamic memory allocated in \p game.
 * 
 * @param game  Pointer to game to free.
 */
void freeGame(game_t* game);

/**
 * @brief Adds troops to a given territory.
 * 
 * @param territory Pointer to territory to add troops to.
 * @param numTroops Number of troops to be added.
 */
void addTroops(territory_t* territory, int numTroops);

/**
 * @brief Removes troops from a given territory.
 * 
 * @param territory Pointer to territory to remove troops from.
 * @param numTroops Number of troops to be removed.
 */
void removeTroops(territory_t* territory, int numTroops);

/**
 * @brief Transfers \p numMove troops from \p from to \p to.
 * 
 * @param from Pointer to territory to remove troops from.
 * @param to Pointer to territory to add troops to.
 * @param numMove Number of troops to transfer.
 */
void transferTroops(territory_t* from, territory_t* to, int numMove);

/**
 * @brief Calculates the number of troops a user should gain.
 * 
 * @param user Pointer to user to calculate gain.
 * 
 * @return Number of troops the user should gain.
 */
int gain(user_t* user);

/*====TERRITORY====*/
/**
 * @brief Initializes \p territories array as unowned.
 * 
 * @param territories pointer to territory array.
 * @param mTerritories number of territories to initialize.
 * @param file opened appvar at correct pointer location.
 */
void initTerritories(territory_t** territories, uint8_t mTerritories, ti_var_t file);

/**
 * @brief Frees an individual territory's dynamic memory as well as \p territory.
 * 
 * @param territory Pointer to territory to free.
 */
void freeTerritory(territory_t* territory);

/**
 * @brief Frees all territories in a territory array.
 * 
 * @param territories Pointer to territories array.
 * @param nTerritories Number of territories to free.
 */
void freeAllTerritories(territory_t** territories, uint8_t nTerritories);

/**
 * @brief Adds a territory to given user's owned territories. Adjusts \p palette to align with user's color.
 * 
 * @param user Pointer to user gaining territory.
 * @param territory Pointer to territory to gain.
 * @param map Pointer to game map.
 * @param palette Pointer to game palette.
 */
void gainTerritory(user_t* user, territory_t* territory, gfx_sprite_t* map, uint8_t* palette);

/**
 * @brief Removes a territory from a user's owned territories.
 * 
 * @param user Pointer to user losing territory.
 * @param territory Pointer to territory to lose.
 */
void loseTerritory(user_t* user, territory_t* territory);

/**
 * @brief Prints the troop count of territory centered at \p x, \p y.
 * 
 * @param territory Pointer to territory to have troop count printed.
 */
void printTerritoryTroops(territory_t* territory);

/*====CONTINENT====*/
/**
 * @brief Initializes continents using data from \p file.
 * 
 * @param continents Pointer to array of continents
 * @param nContinents Number of continents.
 * @param file Slot to read from. Must be pointing to start of continent information.
 */
void initContinents(continent_t** continents, uint8_t nContinents, ti_var_t file);

/**
 * @brief Frees a continent from memory.
 * 
 * @param continent Pointer to continent to free.
 */
void freeContinent(continent_t* continent);

/**
 * @brief Frees an array of continent from memory. DOES NOT free array.
 * 
 * @param continents Pointer to array of continents to free.
 * @param nContinents Number of continents to free.
 */
void freeAllContinents(continent_t** continents, uint8_t nContinents);

/**
 * @brief Checks if \p user owns \p continent and assigns owner if owned.
 * 
 * @param user Pointer to user to check.
 * @param continent Pointer to continent to check.
 * @return true if user owns continent, false if user does not.
 */
bool checkContinent(user_t* user, continent_t* continent);
void updateContinentOwnership(continent_t** continents, uint8_t nContinents, user_t** users, uint8_t nUsers);
void setOwnedContinent(user_t* user, continent_t* continent);
void setNullContinent(continent_t* continent);

/*====USER====*/
void initUsers(user_t** users, uint8_t nUsers, uint8_t gameTerritories);
void freeUser(user_t* user);
void freeAllUsers(user_t** users, uint8_t nUsers);
int userGain(user_t* user, continent_t** continents);

/*====TESTING====*/
void dispTerritory(territory_t* territory, int x, int y);
void dispContinent(continent_t* continent, int x, int y);
void dispUser(user_t* user, int x, int y);
void runTestFile();

/*====OTHER====*/
/**
 * @brief Initializes graphics for a game. Initializes mini-font and runs gfx_Begin().
 * 
 */
void initGraphics();


#endif