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

int main(void)
{
    
    srand(rtc_Time());
    initGraphics();
    gfx_Begin();
    runTestFile();
    gfx_End();
    
    return 0;
}
