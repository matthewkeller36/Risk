#include "input.h"
#include "graphics.h"

int8_t selectUserTerritory(territory_t *territories, uint8_t *territoryList, uint8_t nTerritories, int8_t *choice){
    uint8_t index;
    index = territories[territoryList[*choice]].palIndex;
    gfx_palette[index] = gfx_palette[(COLORS_BEFORE_PLAYERS + territories[territoryList[*choice]].owner->id + 6)];
    kb_scan_edge();
    if(kb_Rising[7]){
        gfx_palette[index] = gfx_palette[(COLORS_BEFORE_PLAYERS + territories[territoryList[*choice]].owner->id)];
        if((kb_Rising[7] & kb_Right) || (kb_Rising[7] & kb_Up)) *choice = (*choice + 1) % nTerritories;
        if((kb_Rising[7] & kb_Left) || (kb_Rising[7] & kb_Down)) *choice = (*choice + nTerritories - 1) % nTerritories;
    }
    if(kb_Rising[1] & kb_2nd){
        gfx_palette[index] = gfx_palette[(COLORS_BEFORE_PLAYERS + territories[territoryList[*choice]].owner->id)];
        return 1;
    }
    if(kb_Rising[6] & kb_Clear){
        if(exitConfirm()){
            gfx_End();
            exit(0);
        }
    }
    
    return 0;
}