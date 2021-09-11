#include "types.h"

void runTestFile(){
    game_t* game = malloc(sizeof(game_t));
    int i, j;
    initGame(game, "RTEST", 6);
    randomAssignTerritories(game);
    // for(i = 0; i < game->nTerritories; i++){
    //     dispTerritory(game->territories[i], 2, 2);
    // }
    // j = i;
    // for(i = 0; i < game->nContinents; i++){
    //     dispContinent(game->continents[i], 2, 2);
    // }
    // dispUser(game->users[0], 4, 4);
    gfx_SetPalette(game->palette, game->paletteSize, 0);

    gainTerritory(game->users[0], game->territories[4], game->map, game->palette);
    // dispUser(game->users[0], 4, 4);
    gfx_SetPalette(game->palette, game->paletteSize, 0);
    gfx_ScaledSprite_NoClip(game->map, 0, 0, 2, 2);
    for(i = 0; i < game->nTerritories; i++){
        printTerritoryTroops(game->territories[i]);
    }
    while(!(os_GetCSC()));
    freeGame(game);
}

void dispTerritory(territory_t* territory, int x, int y){
    int i;
    gfx_FillScreen(255);
    gfx_PrintStringXY("Name: ", x, y);
    gfx_PrintString(territory->name);
    gfx_PrintStringXY("X Pos: ", x, y + 12);
    gfx_PrintUInt(territory->x, 3);
    gfx_PrintStringXY("Y Pos: ", x, y + 24);
    gfx_PrintUInt(territory->y, 3);
    gfx_PrintStringXY("Owner: ", x, y + 36);
    if(!(territory->owner)){
        gfx_PrintChar('N');
    }else{
        gfx_PrintInt(territory->owner->id, 1);
    }
    gfx_PrintStringXY("Troops: ", x, y + 48);
    gfx_PrintUInt(territory->nTroops, 3);
    gfx_PrintStringXY("Connections: ", x, y + 60);
    gfx_PrintUInt(territory->nConnections, 2);
    gfx_PrintStringXY("Conn Indexes: ", x, y + 72);
    for(i = 0; i < territory->nConnections; i++){
        gfx_PrintUInt(territory->connIndexes[i], 2);
        gfx_PrintChar(' ');
    }
    while(!os_GetCSC());
}

void dispContinent(continent_t* continent, int x, int y){
    int i;
    gfx_FillScreen(255);
    gfx_PrintStringXY("Name: ", x, y);
    gfx_PrintString(continent->name);
    gfx_PrintStringXY("Owner: ", x, y + 12);
    if(!(continent->owner)){
        gfx_PrintChar('N');
    }else{
        gfx_PrintInt(continent->owner->id, 1);
    }
    gfx_PrintStringXY("Bonus: ", x, y + 24);
    gfx_PrintUInt(continent->bonus, 2);
    gfx_PrintStringXY("# Territories: ", x, y + 36);
    gfx_PrintInt(continent->nTerritories, 2);
    gfx_PrintStringXY("Territories: ", x, y + 48);
    for(i = 0; i < continent->nTerritories; i++){
        gfx_PrintUInt(continent->territories[i], 2);
        gfx_PrintChar(' ');
    }
    while(!(os_GetCSC()));
}

void dispUser(user_t* user, int x, int y){
    int i;
    gfx_FillScreen(255);
    gfx_PrintStringXY("ID: ", x, y);
    gfx_PrintUInt(user->id, 2);
    gfx_PrintStringXY("# Territories: ", x, y + 12);
    gfx_PrintUInt(user->nTerritories, 2);
    gfx_PrintStringXY("Territories: ", x, y + 24);
    for(i = 0; i < user->nTerritories; i++){
        gfx_PrintUInt(user->userTerritories[i], 2);
        gfx_PrintChar(' ');
    }
    gfx_PrintStringXY("# Continents: ", x, y + 36);
    gfx_PrintUInt(user->nTerritories, 2);
    gfx_PrintStringXY("Continents: ", x, y + 48);
    gfx_PrintStringXY("New Troops: ", x, y + 60);
    gfx_PrintUInt(user->newTroops, 2);
    while(!(os_GetCSC()));
}