#include "graphics.h"

void printAll(game_t* game){
    gfx_FillScreen(WATER_INDEX);
    printMap(game);
    printTopBanner(game);
    printLeftBanner(game);
    printBottomBanner(game);
    printRightBanner(game);
}

void printMap(game_t* game){
    uint8_t i;
    gfx_ScaledSprite_NoClip(game->map, MAP_X_OFFSET, MAP_Y_OFFSET, 2, 2);
    for(i = 0; i < game->nTerritories; i++){
        printTerritoryTroops(game->territories[i]);
    }
}

void printTerritoryTroops(territory_t territory){
    int troopsCopy = territory.nTroops;
    int printX, printY;
    uint8_t i = 0, numLength = 0;
    char num[10];
    while(troopsCopy > 0){
        numLength += 5;
        num[i] = troopsCopy % 10;
        troopsCopy /= 10;
        i++;
    }
    printX = territory.x * 2 + MAP_X_OFFSET - numLength / 2;
    printY = territory.y * 2 + MAP_Y_OFFSET - 2;
    gfx_SetColor(0);
    gfx_Rectangle(printX - 2, printY - 2, numLength + 4, MINI_TEXT_HEIGHT + 4);
    gfx_SetColor(COLORS_BEFORE_PLAYERS + territory.owner->id);
    gfx_FillRectangle(printX - 1, printY - 1, numLength + 2, MINI_TEXT_HEIGHT + 2);
    gfx_SetTextXY(printX, printY);
    for(;i > 0; i--){
        gfx_PrintChar(num[i - 1] + 1);
    }
}

void printTopBanner(game_t* game){
    gfx_SetColor(BLACK_INDEX);
    gfx_PrintStringXY("Turn: ", 2, 1);
    gfx_PrintInt(game->turn, 1);
    gfx_PrintStringXY(game->name, (LCD_WIDTH - gfx_GetStringWidth(game->name)) / 2, 1);
    gfx_PrintStringXY("Fortify", LCD_WIDTH - gfx_GetStringWidth("Fortify") - 2, 1);
    gfx_HorizLine_NoClip(0, MAP_Y_OFFSET - 1, LCD_WIDTH);
}

void printLeftBanner(game_t* game){
    uint8_t i;
    gfx_SetColor(BLACK_INDEX);
    gfx_VertLine_NoClip(MAP_X_OFFSET - 1, MAP_Y_OFFSET, MAP_HEIGHT * 2);
    for(i = 0; i < game->nUsers; i++){
        gfx_SetColor(COLORS_BEFORE_PLAYERS + i);
        gfx_FillCircle_NoClip(MAP_X_OFFSET - 11, MAP_Y_OFFSET + 10 * i + 7, 4);
        if(i == game->playerTurn) gfx_SetColor(WHITE_INDEX);
        else gfx_SetColor(BLACK_INDEX);
        gfx_Circle_NoClip(MAP_X_OFFSET - 11, MAP_Y_OFFSET + 10 * i + 7, 4);
    }
}

void printBottomBanner(game_t* game){
    gfx_SetColor(BLACK_INDEX);
    gfx_HorizLine_NoClip(0, MAP_Y_OFFSET + MAP_HEIGHT * 2, LCD_WIDTH);
    gfx_HorizLine_NoClip(0, MAP_Y_OFFSET + MAP_HEIGHT * 2 + 11, LCD_WIDTH);
    
    gfx_PrintStringXY("Other text", 20, LCD_HEIGHT - 10);
}

void printRightBanner(game_t* game){
    gfx_SetColor(BLACK_INDEX);
    gfx_VertLine_NoClip(MAP_X_OFFSET + MAP_WIDTH * 2, MAP_Y_OFFSET, MAP_HEIGHT * 2);
}