#include "types.h"

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
    gfx_SetColor(3 + territory->owner->id);
    gfx_FillRectangle(printX - 1, printY - 1, numLength + 2, MINI_TEXT_HEIGHT + 2);
    gfx_SetTextXY(printX, printY);
    for(;i > 0; i--){
        gfx_PrintChar(num[i - 1] + 1);
    }
}