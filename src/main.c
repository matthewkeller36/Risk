/*
 *--------------------------------------
 * Program Name: MazeDash
 * Author: matkeller19
 * License:
 * Description:
 *--------------------------------------
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>

/* Standard headers (recommended) */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dice.h"
#include "gfx/gfx.h"

int main(void)
{
    gfx_Begin();
    gfx_SetPalette(global_palette, 4, 0);
    gfx_SetTextBGColor(0);
    gfx_SetTextFGColor(1);
    gfx_SetTextTransparentColor(0);
    while(!os_GetCSC()){
        gfx_ScaledTransparentSprite_NoClip(RiskMap, 0, 5, 2, 2);
        gfx_PrintStringXY("123", 238, 41);
    }
    gfx_End();
}
