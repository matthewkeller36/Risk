#ifndef DICE_H
#define DICE_H

#include "types.h"

bool singleAttack(territory_t* attacker, territory_t* defender);
bool limitRoll(territory_t* attacker, territory_t* defender, unsigned int limit);
bool blitzRoll(territory_t* attacker, territory_t* defender);

#endif