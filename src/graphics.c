#include "graphics.h"
#include <string.h>

int fpsCounter(){
    
    timer_Disable(1);
    float fps = (float)(32678 / timer_GetSafe(1, TIMER_UP));
    timer_Set(1, 0);
    timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_UP);
    return (int)fps;
}

void printAll(game_t *game){
    gfx_FillScreen(WATER_INDEX);
    printMap(game);
    printTopBanner(game);
    printLeftBanner(game);
    printBottomBanner(game);
    printRightBanner(game);
}

void printMap(game_t *game){
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

void printTopBanner(game_t *game){
    gfx_SetColor(BLACK_INDEX);
    gfx_PrintStringXY("Turn: ", 2, 1);
    gfx_PrintInt(game->turn, 1);
    gfx_PrintStringXY(game->name, (LCD_WIDTH - gfx_GetStringWidth(game->name)) / 2, 1);
    gfx_PrintStringXY("Fortify", LCD_WIDTH - gfx_GetStringWidth("Fortify") - 2, 1);
    gfx_HorizLine_NoClip(0, MAP_Y_OFFSET - 1, LCD_WIDTH);
}

void printLeftBanner(game_t *game){
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

void printBottomBanner(game_t *game){
    gfx_SetColor(BLACK_INDEX);
    gfx_HorizLine_NoClip(0, MAP_Y_OFFSET + MAP_HEIGHT * 2, LCD_WIDTH);
    gfx_HorizLine_NoClip(0, MAP_Y_OFFSET + MAP_HEIGHT * 2 + 11, LCD_WIDTH);
    gfx_PrintStringXY("FPS: ", 20, LCD_HEIGHT - 10);
    gfx_PrintInt(fpsCounter(), 2);
}

void printRightBanner(game_t *game){
    gfx_SetColor(BLACK_INDEX);
    gfx_VertLine_NoClip(MAP_X_OFFSET + MAP_WIDTH * 2, MAP_Y_OFFSET, MAP_HEIGHT * 2);
}

void revstr(char *str){
    uint8_t length = strlen(str);
    for(uint8_t i = 0; i < length / 2; i++){
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

void itoa_custom(char *buffer, uint8_t num){
    uint8_t i = 0;
    do{
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }while(num);
    revstr(buffer);
}

void printCentered(char *str, int y){
    gfx_PrintStringXY(str, (LCD_WIDTH - gfx_GetStringWidth(str)) / 2, y);
}