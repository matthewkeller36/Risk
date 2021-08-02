#ifndef RISK_H
#define RISK_H

#include <stdbool.h>
#include <stdint.h>
#include <fileioc.h>
#include <graphx.h>

#define MAP_WIDTH 160
#define MAP_HEIGHT 120

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
bool singleAttack(territory_t*, territory_t*);
bool limitRoll(territory_t*, territory_t*, int);
bool blitzRoll(territory_t*, territory_t*);

/*====GAME====*/
void readGameData(game_t*, char*);
void freeGame(game_t*);
void addTroops(territory_t*, int);
void removeTroops(territory_t*, int);
void transferTroops(territory_t*, territory_t*, int);
int gain(user_t*);

/*====TERRITORY====*/
void initTerritories(territory_t**, uint8_t, ti_var_t);
void freeTerritory(territory_t*);
void freeAllTerritories(territory_t**, uint8_t);
void gainTerritory(user_t*, territory_t*);
void loseTerritory(user_t*, territory_t*);

/*====CONTINENT====*/
void initContinents(continent_t**, uint8_t, ti_var_t);
void freeContinent(continent_t*);
void freeAllContinents(continent_t**, uint8_t);
bool checkContinent(user_t*, continent_t*);
void gainContinent(user_t*, continent_t*);
void loseContinent(user_t*, continent_t*);

/*====USER====*/
void initNewUser();

/*====TESTING====*/
void dispTerritory(territory_t*, int, int);
void dispContinent(continent_t*, int, int);
void runTestFile();

#endif