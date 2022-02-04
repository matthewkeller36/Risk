#ifndef ISSUE_TYPES_H
#define ISSUE_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <fileioc.h>
#include <graphx.h>
#include "key_helper.h"

#define MAX_TERRITORIES 64
#define MAX_TERR_IN_CONTINENT 10
#define MAX_CONNS 8
#define MAX_CONTINENTS 16
#define MAX_USERS 6

#define MAP_WIDTH 140
#define MAP_HEIGHT 100
#define MAP_X_OFFSET 0
#define MAP_Y_OFFSET 0

#define MINI_TEXT_HEIGHT 5

#define COLORS_BEFORE_PLAYERS 3


typedef struct {
    uint8_t id;
    uint8_t nTerritories;
    uint8_t userTerritories[MAX_TERRITORIES];
    uint8_t newTroops;
}user_t;

typedef struct {
    uint8_t id;
    char name[20];
    user_t* owner;
    uint8_t bonus;
    uint8_t nTerritories;
    uint8_t territories[MAX_TERR_IN_CONTINENT];
}continent_t;

typedef struct {
    uint8_t id;
    char name[20];
    unsigned int x;
    unsigned int y;
    user_t* owner;
    uint8_t continent;
    
    uint8_t nConnections;
    int nTroops;
    uint8_t connIndexes[MAX_CONNS];
    uint8_t palIndex;
}territory_t;

typedef struct {
    uint8_t nameLength;
    char name[20];
    gfx_sprite_t* map;
    uint8_t nTerritories;
    territory_t territories[MAX_TERRITORIES];
    uint8_t nContinents;
    continent_t continents[MAX_CONTINENTS];
    uint8_t nUsers;
    user_t users[MAX_USERS];
    unsigned char palette[256];
    uint8_t paletteSize;
}game_t;

#endif