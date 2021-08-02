#include <string.h>

#include <tice.h>
#include <fileioc.h>
#include <graphx.h>
#include "types.h"
#include <debug.h>
#include <compression.h>

/*====GAME====*/
void readGameData(game_t* game, char* fileName){
    ti_var_t file;
    int i;
    gfx_Begin();
    ti_CloseAll();
    if(!(file = ti_Open(fileName, "r"))){
        return;
    }
    ti_Seek(4, 0, file);
    ti_Read(&game->nameLength, sizeof(uint8_t), 1, file);
    game->name = malloc(sizeof(char) * game->nameLength);
    ti_Read(game->name, sizeof(char), game->nameLength, file);
    

    // game->map = gfx_MallocSprite(MAP_WIDTH, MAP_HEIGHT);
    // ti_Read(game->map, sizeof(game->map), 1, file);
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
    memcpy(game->palette, (unsigned char[]){0x00, 0x00, 0xbe, 0x02, 0xff, 0xff, 0x00, 0x7c, 0xe0, 0x83, 0x1f, 0x00, 0xe0, 0xff, 0x1f, 0x7c, 0xff, 0x83}, 18);
    for(i = 17; i < game->paletteSize; i += 2){
        game->palette[i] = 0x10;
        game->palette[++i] = 0x42;
    }
}

void freeGame(game_t* game){
    free(game->name);
    free(game->map);
    freeAllTerritories(game->territories, game->nTerritories);
    free(game->territories);
    free(game->map);
    free(game->palette);
    freeAllContinents(game->continents, game->nContinents);
    free(game->continents);
    free(game->users);
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
        ti_Read(&(thisTerritory->x), 1, 1, file);
        ti_Read(&(thisTerritory->y), 1, 1, file);
        thisTerritory->x *= 2;
        thisTerritory->y *= 2;
        thisTerritory->owner = NULL;
        ti_Read(&(thisTerritory->continent), sizeof(uint8_t), 1, file);
        thisTerritory->nTroops = 0;
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
        
        // gfx_Begin();
        // gfx_SetTextXY(4,4);
        // gfx_PrintInt(nameLength, 2);
        // while(!os_GetCSC());
        
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
void initNewUser(){

}