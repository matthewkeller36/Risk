#include "types.h"

void printMap(game_t* game){
    uint8_t i;
    gfx_ScaledSprite_NoClip(game->map, MAP_X_OFFSET, MAP_Y_OFFSET, 2, 2);
    for(i = 0; i < game->nTerritories; i++){
        printTerritoryTroops(game->territories[i]);
    }
}

void printTerritoryTroops(territory_t* territory){
    int troopsCopy = territory->nTroops, printX, printY;
    int8_t i = 0, numLength = 0;
    char num[10];

    while(troopsCopy > 0){
        numLength += 5;
        num[i] = troopsCopy % 10;
        troopsCopy /= 10;
        i++;
    }
    printX = territory->x * 2 + MAP_X_OFFSET - numLength / 2;
    printY = territory->y * 2 + MAP_Y_OFFSET - 2;
    gfx_SetColor(0);
    gfx_Rectangle(printX - 2, printY - 2, numLength + 4, MINI_TEXT_HEIGHT + 4);
    gfx_SetColor(COLORS_BEFORE_PLAYERS + territory->owner->id);
    gfx_FillRectangle(printX - 1, printY - 1, numLength + 2, MINI_TEXT_HEIGHT + 2);
    gfx_SetTextXY(printX, printY);
    for(;i > 0; i--){
        gfx_PrintChar(num[i - 1] + 1);
    }
}