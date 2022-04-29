#ifndef KEY_HELPER
#define KEY_HELPER

#include <stdint.h>
#include <stdbool.h>

#include <keypadc.h>

void kb_scan_edge();
void kb_scan_rising();
void kb_scan_falling();
void init_keys();

extern uint8_t kb_Falling[8];
extern uint8_t kb_Rising[8];

#endif