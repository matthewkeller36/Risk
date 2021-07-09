#include "types.h"

void dispTerritory(territory_t* territory, int x, int y){
    int i;
    gfx_PrintStringXY(territory->name, x, y);
    gfx_PrintChar(' ');
    gfx_PrintInt(territory->x, 3);
    gfx_PrintChar(' ');
    gfx_PrintInt(territory->y, 3);
    gfx_PrintChar(' ');
    if(!(territory->owner)){
        gfx_PrintInt(territory->owner->id, 1);
    }else{
        gfx_PrintChar('N');
    }
    gfx_PrintChar(' ');
    gfx_PrintInt(territory->nTroops, 3);
    gfx_PrintChar(' ');
    gfx_PrintInt(territory->nConnections, 2);
    gfx_PrintChar(' ');
    for(i = 0; i < territory->nConnections; i++){
        gfx_PrintInt(territory->connIndexes, 2);
        gfx_PrintChar(' ');
    }
}

void dispContinent(continent_t* continent, int x, int y){
    int i;
    gfx_PrintStringXY(continent->name, x, y);
    gfx_PrintChar(' ');
    if(!(continent->owner)){
        gfx_PrintInt(continent->owner->id, 1);
    }else{
        gfx_PrintChar('N');
    }
    gfx_PrintChar(' ');
    gfx_PrintInt(continent->bonus, 2);
    gfx_PrintChar(' ');
    gfx_PrintInt(continent->nTerritories, 2);
    for(i = 0; i < continent->nTerritories; i++){
        gfx_PrintInt(continent->territories, 2);
        gfx_PrintChar(' ');
    }
}