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

#include "types.h"
#include "graphics.h"

int main(void)
{
    static game_t game;
    srand(rtc_Time());
    initGraphics();
    timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_UP);
    runTestFile(&game);
    gfx_End();
    
    return 0;
}
