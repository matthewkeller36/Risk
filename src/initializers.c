#include <string.h>

#include <tice.h>
#include <fileioc.h>
#include <graphx.h>
#include "types.h"
#include <debug.h>
#include <compression.h>

/*====GAME====*/
bool initGame(game_t *game, char* fileName, uint8_t nUsers){
    ti_var_t file;
    int i;
    
    uint8_t pal[30] = {
        0x00, 0x00, 0xbe, 0x02, 0xff, 0xff, //Black, light blue, white
        0x00, 0x64, 0x80, 0x02, 0xde, 0x31, //Red, green, blue
        0x20, 0x67, 0x19, 0x60, 0x39, 0x03, //Yellow, magenta, cyan
        0x00, 0x7c, 0xe0, 0x23, 0x1f, 0x00, //Light variants of all the user colors in order
        0x80, 0xff, 0x1f, 0x7c, 0xff, 0x83};
    if(!(file = ti_Open(fileName, "r"))){
        return false;
    }
    dbg_sprintf(dbgout, "1 \n");
    ti_Seek(4, 0, file);
    ti_Read(&game->nameLength, sizeof(uint8_t), 1, file);
    dbg_sprintf(dbgout, "%d \n", game->nameLength);
    ti_Read(game->name, sizeof(char), game->nameLength, file);
    
    ti_Read(&(game->nTerritories), sizeof(uint8_t), 1, file);

    game->nUsers = nUsers;
    initUsers(game->users, game->nUsers);
    dbg_sprintf(dbgout, "Initializing Territories \n");
    initTerritories(game->territories, game->nTerritories, file);
    dbg_sprintf(dbgout, "Initializing Territories Complete \n");
    ti_Read(&(game->nContinents), 1, 1, file);
    dbg_sprintf(dbgout, "Initializing Continents \n");
    initContinents(game->continents, game->nContinents, file);
    dbg_sprintf(dbgout, "Initializing Continents Complete \n");
    game->map = gfx_MallocSprite(MAP_WIDTH, MAP_HEIGHT);
    zx7_Decompress(game->map, ti_GetDataPtr(file));
    ti_Close(file);
    game->paletteSize = 2 * game->nTerritories + 30;
    for(i = 0; i < 30; i++){
        game->palette[i] = pal[i];
    }
    for(i = 30; i < game->paletteSize; i++){
        game->palette[i] = 0x10;
        game->palette[++i] = 0x42;
    }
    gfx_SetPalette(game->palette, game->paletteSize, 0);
    for(i = 0; i < game->nTerritories; i++){
        game->territories[i].palIndex = game->map->data[game->territories[i].x + game->territories[i].y * MAP_WIDTH];
    }
    game->turn = 1;
    game->playerTurn = 0;
    return true;
}

void randomAssignTerritories(game_t *game){
    uint8_t i, j;
    uint8_t baseArray[MAX_TERRITORIES];
    for(i = 0; i < game->nTerritories; i++){
        baseArray[i] = i;
    }
    for(i = 0; i < game->nTerritories; i++){
        uint8_t temp, newIndex;
        newIndex = randInt(i, game->nTerritories - 1);
        temp = baseArray[i];
        baseArray[i] = baseArray[newIndex];
        baseArray[newIndex] = temp;
        gainTerritory(&game->users[i % game->nUsers], &game->territories[baseArray[i]]);
    }
    for(i = 0; i < game->nUsers; i++){
        for(j = 0; j < 50 - 5 * game->nUsers - game->users[i].nTerritories; j++){
            addTroops(&game->territories[game->users[i].userTerritories[randInt(0, game->users[i].nTerritories - 1)]], 1);
        }
    }
}

/*====Territory====*/
void initTerritories(territory_t *territories, uint8_t nTerritories, ti_var_t file){
    int id;

    for(id = 0; id < nTerritories; id++){
        uint8_t nameLength = 0;
        territories[id].id = id;
        ti_Read(&nameLength, sizeof(nameLength), 1, file);
        ti_Read(territories[id].name, sizeof(char), nameLength, file);
        territories[id].x = territories[id].y = 0;
        ti_Read(&(territories[id].x), 1, 1, file);
        ti_Read(&(territories[id].y), 1, 1, file);
        territories[id].owner = NULL;
        territories[id].nTroops = 1;
        ti_Read(&(territories[id].continent), sizeof(uint8_t), 1, file);
        ti_Read(&(territories[id].nConnections), sizeof(uint8_t), 1, file);
        ti_Read(territories[id].connIndexes, sizeof(uint8_t), territories[id].nConnections, file);
    }
}

/*====CONTINENT====*/
bool initContinents(continent_t *continents, uint8_t nContinents, ti_var_t file){
    int id;
    for(id = 0; id < nContinents; id++){
        uint8_t nameLength = 0;
        continents[id].id = id;
        ti_Read(&nameLength, sizeof(nameLength), 1, file);
        ti_Read(continents[id].name, sizeof(char), nameLength, file);
        continents[id].owner = NULL;
        ti_Read(&(continents[id].bonus), sizeof(uint8_t), 1, file);
        ti_Read(&(continents[id].nTerritories), sizeof(uint8_t), 1, file);
        ti_Read(continents[id].territories, sizeof(uint8_t), continents[id].nTerritories, file);
    }
    return true;
}

/*====USER====*/
bool initUsers(user_t *users, uint8_t nUsers){
    for(uint8_t i = 0; i < nUsers; i++){
        users[i].id = i;
        users[i].nTerritories = 0;
        users[i].newTroops = 0;
        for(uint8_t j = 0; j < MAX_CARDS; j++) users[i].cards[j] = 0;
    }
    return true;
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
    gfx_SetDrawBuffer();
}