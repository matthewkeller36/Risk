#include <tice.h>

#include "types.h"

/*Will return true if attacker won, false if defender won*/

bool singleAttack(territory_t* attack, territory_t* def){
    uint8_t attackRolls[3] = {0, 0, 0};
    uint8_t defendRolls[2] = {0, 0};
    uint8_t temp;

    attackRolls[0] = randInt(1, 6);
    if(attack->nTroops > 2) attackRolls[1] = randInt(1, 6);
    if(attack->nTroops > 3) attackRolls[2] = randInt(1, 6);

    defendRolls[0] = randInt(1, 6);
    if(def->nTroops > 1) defendRolls[1] = randInt(1, 6);

    for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			if (attackRolls[j] > attackRolls[i]){
				temp = attackRolls[i];
				attackRolls[i] = attackRolls[j];
				attackRolls[j] = temp;
			}  
		}
    }
    if(defendRolls[0] < defendRolls[1]){
        temp = defendRolls[0];
        defendRolls[0] = defendRolls[1];
        defendRolls[1] = temp;
    }

    if(attackRolls[0] > defendRolls[0]) def->nTroops--;
    else attack->nTroops--;

    if(attackRolls[1] * defendRolls[1] != 0){
        if(attackRolls[1] > defendRolls[1]) def->nTroops--;
        else attack->nTroops--;
    }

    return def->nTroops < 1;
    
}

bool limitRoll(territory_t* attack, territory_t* def, int limit){
    while(attack->nTroops > limit && def->nTroops > 0){
        singleAttack(attack, def);
    }
    return def->nTroops < 1;
}

bool blitzRoll(territory_t* attack, territory_t* def){
    return limitRoll(attack, def, 1);
}