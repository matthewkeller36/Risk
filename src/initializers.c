#include <string.h>

#include <tice.h>
#include <fileioc.h>
#include <graphx.h>
#include "types.h"
#include <debug.h>
#include <compression.h>

/*====GAME====*/
void initGame(game_t* game, char* fileName, uint8_t nUsers){
    ti_var_t file;
    int i;
    ti_CloseAll();
    if(!(file = ti_Open(fileName, "r"))){
        return;
    }
    ti_Seek(4, 0, file);
    ti_Read(&game->nameLength, sizeof(uint8_t), 1, file);
    game->name = malloc(sizeof(char) * game->nameLength);
    ti_Read(game->name, sizeof(char), game->nameLength, file);
    ti_Read(&(game->nTerritories), sizeof(uint8_t), 1, file);
    game->territories = malloc(game->nTerritories * sizeof(territory_t*));
    initTerritories(game->territories, game->nTerritories, file);

    ti_Read(&(game->nContinents), 1, 1, file);

    game->continents = malloc(game->nContinents * sizeof(continent_t*));
    initContinents(game->continents, game->nContinents, file);
    game->map = gfx_MallocSprite(MAP_WIDTH, MAP_HEIGHT);
    zx7_Decompress(game->map, ti_GetDataPtr(file));
    ti_CloseAll();
    game->paletteSize = 2 * game->nTerritories + 18;
    game->palette = malloc(game->paletteSize);
    memcpy(game->palette, (unsigned char[]){
        0x00, 0x00, 0xbe, 0x02, 0xff, 0xff, 
        0x00, 0x7c, 0xe0, 0x83, 0xde, 0x31, 
        0xe0, 0xff, 0x1f, 0x7c, 0xff, 0x83}, 18);
    for(i = 18; i < game->paletteSize; i++){
        game->palette[i] = 0x10;
        game->palette[++i] = 0x42;
    }
    game->nUsers = nUsers;
    game->users = malloc(game->nUsers * sizeof(user_t*));
    initUsers(game->users, game->nUsers, game->nTerritories);
    
}

void randomAssignTerritories(game_t* game){
    uint8_t i;
    uint8_t* baseArray;
    baseArray = malloc(game->nTerritories);
    for(i = 0; i < game->nTerritories; i++){
        baseArray[i] = i;
    }
    for(i = 0; i < game->nTerritories; i++){
        uint8_t temp, newIndex;
        newIndex = randInt(i, game->nTerritories - 1);
        temp = baseArray[i];
        baseArray[i] = baseArray[newIndex];
        baseArray[newIndex] = temp;
        gainTerritory(game->users[i % game->nUsers], game->territories[baseArray[i]], game->map, game->palette);
    }
}

void freeGame(game_t* game){
    free(game->name);
    free(game->map);
    freeAllTerritories(game->territories, game->nTerritories);
    free(game->territories);
    free(game->palette);
    freeAllContinents(game->continents, game->nContinents);
    free(game->continents);
    freeAllUsers(game->users, game->nUsers);
    // free(game->users);
    free(game);
}

/*====Territory====*/
void initTerritories(territory_t** territories, uint8_t nTerritories, ti_var_t file){
    int id;

    for(id = 0; id < nTerritories; id++){
        territory_t* thisTerritory = malloc(sizeof(territory_t));
        uint8_t nameLength = 0;
        thisTerritory->id = id;
        ti_Read(&nameLength, sizeof(nameLength), 1, file);
        thisTerritory->name = malloc(nameLength);
        ti_Read(thisTerritory->name, sizeof(char), nameLength, file);
        thisTerritory->x = thisTerritory->y = 0;
        ti_Read(&(thisTerritory->x), 1, 1, file);
        ti_Read(&(thisTerritory->y), 1, 1, file);
        thisTerritory->owner = NULL;
        ti_Read(&(thisTerritory->continent), sizeof(uint8_t), 1, file);
        thisTerritory->nTroops = 1;
        ti_Read(&(thisTerritory->nConnections), sizeof(uint8_t), 1, file);
        thisTerritory->connIndexes = malloc(thisTerritory->nConnections);
        ti_Read(thisTerritory->connIndexes, sizeof(uint8_t), thisTerritory->nConnections, file);
        territories[id] = thisTerritory;
    }
    
}

void freeTerritory(territory_t* territory){
    free(territory->name);
    free(territory->connIndexes);
    free(territory);
}

void freeAllTerritories(territory_t** territories, uint8_t nTerritories){
    for(uint8_t i = 0; i < nTerritories; i++){
        freeTerritory(territories[i]);
    }
}

/*====CONTINENT====*/
void initContinents(continent_t** continents, uint8_t nContinents, ti_var_t file){
    int id;
    for(id = 0; id < nContinents; id++){
        continent_t* thisContinent = malloc(sizeof(continent_t));
        
        uint8_t nameLength = 0;
        thisContinent->id = id;
        ti_Read(&nameLength, sizeof(nameLength), 1, file);
        thisContinent->name = malloc(nameLength * sizeof(uint8_t));
        ti_Read(thisContinent->name, sizeof(char), nameLength, file);
        thisContinent->owner = NULL;
        ti_Read(&(thisContinent->bonus), sizeof(uint8_t), 1, file);
        ti_Read(&(thisContinent->nTerritories), sizeof(uint8_t), 1, file);
        thisContinent->territories = malloc(thisContinent->nTerritories);
        ti_Read(thisContinent->territories, sizeof(uint8_t), thisContinent->nTerritories, file);
        continents[id] = thisContinent;
    }
}

void freeContinent(continent_t* continent){
    free(continent->name);
    free(continent->territories);
    free(continent);
}

void freeAllContinents(continent_t** continents, uint8_t nContinents){
    for(uint8_t i = 0; i < nContinents; i++){
        freeContinent(continents[i]);
    }
}

/*====USER====*/
void initUsers(user_t** user, uint8_t nUsers, uint8_t gameTerritories){
    for(uint8_t i = 0; i < nUsers; i++){
        user_t* thisUser = malloc(sizeof(user_t));
        thisUser->id = i;
        thisUser->nTerritories = 0;
        thisUser->userTerritories = malloc(gameTerritories);
        thisUser->newTroops = 0;
        user[i] = thisUser;
    }
}

void freeUser(user_t* user){
    free(user->userTerritories);
    free(user);
}

void freeAllUsers(user_t** users, uint8_t nUsers){
    for(uint8_t i = 0; i < nUsers; i++){
        freeUser(users[i]);
    }
}

/*====OTHER====*/


void initGraphics(){
    int i;
    uint8_t miniFont[80] = {
        0x70,0x50,0x50,0x50,0x70,0x00,0x00,0x00,
        0x60,0x20,0x20,0x20,0x70,0x00,0x00,0x00,
        0x70,0x10,0x70,0x40,0x70,0x00,0x00,0x00,
        0x70,0x10,0x70,0x10,0x70,0x00,0x00,0x00,
        0x50,0x50,0x70,0x10,0x10,0x00,0x00,0x00,
        0x70,0x40,0x70,0x10,0x70,0x00,0x00,0x00,
        0x70,0x40,0x70,0x50,0x70,0x00,0x00,0x00,
        0x70,0x10,0x10,0x10,0x10,0x00,0x00,0x00,
        0x70,0x50,0x70,0x50,0x70,0x00,0x00,0x00,
        0x70,0x50,0x70,0x10,0x70,0x00,0x00,0x00
    };

    static uint8_t miniFontSpacing[128] = {
        0,5,5,5,5,5,5,5,5,5,5,8,8,2,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        3,4,6,8,8,8,8,5,5,5,8,7,4,7,3,8,
        8,7,8,8,8,8,8,8,8,8,3,4,6,7,6,7,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,5,8,5,8,8,
        4,8,8,8,8,8,8,8,8,5,8,8,5,8,8,8,
        8,8,8,8,7,8,8,8,8,8,8,7,3,7,8,8
    };
    
    gfx_Begin();
    for(i = 0; i < 10; i++){
        gfx_SetCharData(i + 1, &miniFont[i * 8]);
    }
    gfx_SetFontSpacing(miniFontSpacing);
}