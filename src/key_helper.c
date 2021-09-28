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

uint8_t kb_Falling[8];
uint8_t kb_Rising[8];

void kb_scan_edge(){
    int i;
    static uint8_t kb_Previous[8];
    kb_Scan();
    for(i = 0; i < 8; i++){
        kb_Rising[i] = (kb_Data[i] ^ kb_Previous[i]) & kb_Data[i];
        kb_Falling[i] = (kb_Data[i] ^ kb_Previous[i]) & kb_Data[i];
        kb_Previous[i] = kb_Data[i];
    }
}