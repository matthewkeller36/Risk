#include "graphics.h"
#include <string.h>

int fpsCounterLimit(int target){
    int fps;
    gfx_SetColor(WATER_INDEX);
    gfx_FillRectangle_NoClip(0, LCD_HEIGHT - 10, 64, 10);
    gfx_PrintStringXY("FPS: ", 2, LCD_HEIGHT - 10);
    
    while((fps = (32678 / timer_GetSafe(1, TIMER_UP))) > target);
    timer_Disable(1);
    timer_Set(1, 0);
    timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_UP);
    gfx_PrintInt(fps, 2);
    return fps;
}

void printAll(game_t *game, char *dispText){

    if(game->redraws & REDRAW_MAP) printMap(game);
    if(game->redraws & REDRAW_TOP) printTopBanner(game);
    if(game->redraws & REDRAW_LEFT) printLeftBanner(game);
    if(game->redraws & REDRAW_BOTTOM) printBottomBanner(game, dispText);
    if(game->redraws & REDRAW_RIGHT) printRightBanner(game);
    fpsCounterLimit(240);
    gfx_BlitBuffer();
}

void printMap(game_t *game){
    uint8_t i;
    gfx_ScaledSprite_NoClip(game->map, MAP_X_OFFSET, MAP_Y_OFFSET, 2, 2);
    for(i = 0; i < game->nTerritories; i++){
        printTerritoryTroops(game->territories[i]);
    }
    game->redraws ^= REDRAW_MAP;
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
    game->redraws ^= REDRAW_TOP;
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
    game->redraws ^= REDRAW_LEFT;
}

void printBottomBanner(game_t *game, char *dispText){
    gfx_SetColor(WATER_INDEX);
    gfx_FillRectangle_NoClip(0, MAP_Y_OFFSET + MAP_HEIGHT * 2, LCD_WIDTH, LCD_HEIGHT - (MAP_Y_OFFSET + MAP_HEIGHT * 2));
    gfx_SetColor(BLACK_INDEX);
    gfx_HorizLine_NoClip(0, MAP_Y_OFFSET + MAP_HEIGHT * 2, LCD_WIDTH);
    gfx_HorizLine_NoClip(0, MAP_Y_OFFSET + MAP_HEIGHT * 2 + 11, LCD_WIDTH);
    printCentered(dispText, MAP_Y_OFFSET + 2 * MAP_HEIGHT + 2);
    game->redraws ^= REDRAW_BOTTOM;
}

void printRightBanner(game_t *game){
    gfx_SetColor(WATER_INDEX);
    gfx_FillRectangle_NoClip(MAP_X_OFFSET + MAP_WIDTH * 2, MAP_Y_OFFSET, LCD_WIDTH - (MAP_X_OFFSET + MAP_WIDTH * 2), MAP_HEIGHT * 2);
    gfx_SetColor(BLACK_INDEX);
    gfx_VertLine_NoClip(MAP_X_OFFSET + MAP_WIDTH * 2, MAP_Y_OFFSET, MAP_HEIGHT * 2);
    game->redraws ^= REDRAW_RIGHT;
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

uint8_t exitConfirm(){
    uint8_t choice = 0;
    gfx_SetColor(WATER_INDEX);
    gfx_FillRectangle_NoClip((LCD_WIDTH - 160) / 2, (LCD_HEIGHT - 64) / 2, 160, 65);
    gfx_SetColor(BLACK_INDEX);
    gfx_Rectangle_NoClip((LCD_WIDTH - 162) / 2, (LCD_HEIGHT - 66) / 2, 161, 66);
    printCentered("Are you sure you", (LCD_HEIGHT - 60) / 2);
    printCentered("would like to quit?", (LCD_HEIGHT - 40) / 2);
    printCentered("Your game will be saved", (LCD_HEIGHT) / 2);
    printCentered("No        Yes", (LCD_HEIGHT + 40) / 2);
    do{
        gfx_SetColor(WATER_INDEX);
        gfx_Rectangle_NoClip((LCD_WIDTH - 78 + choice * 88) / 2, (LCD_HEIGHT + 40) / 2 - 2, 28, 12);
        kb_scan_edge();
        if(kb_Rising[7]){
            choice = (choice + 1) % 2;
        }
        gfx_SetColor(BLACK_INDEX);
        gfx_Rectangle_NoClip((LCD_WIDTH - 78 + choice * 88) / 2, (LCD_HEIGHT + 40) / 2 - 2, 28, 12);
        gfx_BlitBuffer();
    }while(!(kb_Rising[1] & kb_2nd));
    return choice;
}