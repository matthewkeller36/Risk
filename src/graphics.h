#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"

void initGraphics();
void printAll(game_t* game, char *dispText);
void printTerritoryName(territory_t* territory);
void printTopBanner(game_t* game);
void printLeftBanner(game_t* game);
void printBottomBanner(game_t* game, char *dispText);
void printRightBanner(game_t* game);
void itoa_custom(char* buffer, uint8_t num);
void printCentered(char* str, int y);
uint8_t exitConfirm();
#endif