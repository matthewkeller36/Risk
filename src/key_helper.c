#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

/* Standard headers (recommended) */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <keypadc.h>

#include "key_helper.h"

#define dataLoc ((uint16_t*)0xF50010)

uint16_t* kb_Previous;
uint16_t* kb_Falling;
uint16_t* kb_Rising;

void init_Keys(){
    kb_Previous = malloc(8 * sizeof(uint16_t));
    kb_Falling = malloc(8 * sizeof(uint16_t));
    kb_Rising = malloc(8 * sizeof(uint16_t));
    memset(kb_Previous, *dataLoc, 8);
    memset(kb_Falling, 0, 8);
    memset(kb_Rising, 0, 8);
}

void scan_kb(){
    int i;
    memcpy(kb_Previous, dataLoc, 8 * 16);
    kb_Scan();
    for(i = 0; i < 8; i++){
        kb_Rising[i] = (dataLoc[i] ^ kb_Previous[i]) & dataLoc[i];
        kb_Falling[i] = (dataLoc[i] ^ kb_Previous[i]) & kb_Previous[i];
    }
}

void end_kb(){
    free(kb_Previous);
    free(kb_Falling);
    free(kb_Rising);
}