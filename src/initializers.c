#include <tice.h>
#include <fileioc.h>
#include <graphx.h>
#include "types.h"

/*====GAME====*/
void readGameData(game_t* game, char* fileName){
    ti_var_t file;
    int readPos;

    ti_CloseAll();
    if(!(file = ti_Open(fileName, "r"))){
        return;
    }
    ti_Seek(4, 0, file);
    ti_Read(game->nameLength, sizeof(uint8_t), 1, file);
    game->name = malloc(sizeof(char) * game->nameLength);
    ti_Read(game->name, sizeof(char), game->nameLength, file);
    game->map = gfx_MallocSprite(MAP_WIDTH, MAP_HEIGHT);
    ti_Read(game->map, sizeof(game->map), 1, file);
    ti_Read(game->nTerritories, sizeof(uint8_t), 1, file);
    game->territories = malloc(game->nTerritories * sizeof(territory_t*));

    initTerritories(game->territories, game->nTerritories, file);

    initContinents(game->continents, game->nContinents, file);
}

void freeGame(game_t* game){
    free(game->name);
    free(game->map);
    free(game->territories);
    free(game->users);
    free(game);
}

/*====Territory====*/
void initTerritories(territory_t** territories, uint8_t nTerritories, ti_var_t* file){
    int id;

    for(id = 0; id < nTerritories; id++){
        territory_t* thisTerritory;
        uint8_t nameLength;
        thisTerritory->id = id;
        ti_Read(nameLength, sizeof(nameLength), 1, file);
        thisTerritory->name = malloc(nameLength);
        ti_Read(thisTerritory->name, sizeof(char), nameLength, file);
        ti_Read(thisTerritory->x, sizeof(int), 1, file);
        ti_Read(thisTerritory->y, sizeof(int), 1, file);
        thisTerritory->owner = NULL;
        ti_Read(thisTerritory->continent, sizeof(uint8_t), 1, file);
        thisTerritory->nTroops = 0;
        ti_Read(thisTerritory->nConnections, sizeof(uint8_t), 1, file);
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

/*====CONTINENT====*/
void initContinents(continent_t** continents, uint8_t nContinents, ti_var_t* file){
    int id;
    for(id = 0; id < nContinents; id++){
        continent_t* thisContinent;
        uint8_t nameLength;
        thisContinent->id = id;
        ti_Read(nameLength, sizeof(nameLength), 1, file);
        thisContinent->name = malloc(sizeof(nameLength));
        ti_Read(thisContinent->name, sizeof(char), nameLength, file);
        thisContinent->owner = NULL;
        ti_Read(thisContinent->bonus, sizeof(uint8_t), 1, file);
        ti_Read(thisContinent->nTerritories, sizeof(uint8_t), 1, file);
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

/*====USER====*/
void initNewUser(){

}