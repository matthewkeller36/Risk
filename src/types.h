#ifndef RISK_H
#define RISK_H

#include <stdbool.h>
#include <stdint.h>
#include <fileioc.h>
#include <graphx.h>
#include "key_helper.h"

#define MAX_TERRITORIES 64
#define MAX_TERR_IN_CONTINENT 16
#define MAX_CONNS 8
#define MAX_CONTINENTS 8
#define MAX_USERS 6
#define MAX_CARDS 9

#define MAP_WIDTH 255
#define MAP_HEIGHT 200
#define MAP_SCALE 1
#define MAP_X_OFFSET (LCD_WIDTH / 2) - (MAP_WIDTH * MAP_SCALE) / 2
#define MAP_Y_OFFSET LCD_HEIGHT / 2 - (MAP_HEIGHT * MAP_SCALE) / 2 - 9

#define MINI_TEXT_HEIGHT 5

#define COLORS_BEFORE_PLAYERS 3
#define BLACK_INDEX 0
#define WATER_INDEX 1
#define WHITE_INDEX 2

#define REDRAW_MAP (1)
#define REDRAW_RIGHT (1 << 1)
#define REDRAW_TOP (1 << 2)
#define REDRAW_LEFT (1 << 3)
#define REDRAW_BOTTOM (1 << 4)
#define REDRAW_ALL 0x1F



typedef struct {
    uint8_t id;
    uint8_t nTerritories;
    uint8_t userTerritories[MAX_TERRITORIES];
    uint8_t newTroops;
    uint8_t cards[MAX_CARDS];
}user_t;

typedef struct {
    uint8_t id;
    char *name;
    user_t *owner;
    uint8_t bonus;
    uint8_t nTerritories;
    uint8_t *territories;
}continent_t;

typedef struct { //Progressive, pass-through, random assign, 
    
}options_t;

typedef struct {
    uint8_t id;
    char *name;
    unsigned int x;
    unsigned int y;
    uint8_t continent;
    uint8_t nConnections;
    unsigned int nTroops;
    uint8_t *connIndexes;
    uint8_t palIndex;
    user_t *owner;
}territory_t;

typedef struct {
    uint8_t nameLength;
    char *name;
    gfx_sprite_t *map;
    uint8_t nTerritories;
    territory_t territories[MAX_TERRITORIES];
    uint8_t nContinents;
    continent_t continents[MAX_CONTINENTS];
    uint8_t nUsers;
    user_t users[MAX_USERS];
    unsigned char palette[256];
    uint8_t paletteSize;
    int turn;
    uint8_t playerTurn;
    uint8_t redraws;
}game_t;

/*====GAME====*/
bool initGame(game_t *game, char* fileName, uint8_t nUsers);
void randomAssignTerritories(game_t *game);
void printMap(game_t *game);
void draft(game_t *game);
void preAttack(game_t *game);
void attack(game_t *game);
void preFortify(game_t *game);

/*====TERRITORY====*/
void transferTroops(territory_t *from, territory_t *to, int numMove);
void * initTerritories(territory_t *territories, uint8_t mTerritories, void *file_ptr);
void gainTerritory(user_t *user, territory_t *territory);
void loseTerritory(user_t *user, territory_t *territory);
void printTerritoryTroops(territory_t territory);

/*====CONTINENT====*/
void * initContinents(continent_t *continents, uint8_t nContinents, void *file_ptr);
bool checkContinent(user_t user, continent_t continent);
void updateContinentOwnership(continent_t *continents, uint8_t nContinents, user_t *users, uint8_t nUsers);
void setOwnedContinent(user_t *user, continent_t *continent);
void setNullContinent(continent_t *continent);

/*====USER====*/
bool initUsers(user_t *users, uint8_t nUsers);
int userGain(user_t *user);

/*====TESTING====*/
void dispTerritory(territory_t *territory, int x, int y);
void dispContinent(continent_t *continent, int x, int y);
void dispUser(user_t *user, int x, int y);
void runTestFile(game_t *game);

#endif