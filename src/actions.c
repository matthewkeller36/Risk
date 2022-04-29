#include "types.h"
#include "input.h"
#include "graphics.h"
#include <string.h>

/*====GAME====*/

void draft(game_t *game){
    int gain;
    int8_t selectedTerritory = 0;
    updateContinentOwnership(game->continents, game->nContinents, game->users, game->nUsers);
    gain = userGain(&game->users[game->playerTurn]);
    while(gain > 0){
        char dispText[20] = "Remaining: ", buf[5] = {0};
        itoa_custom(buf, gain);
        strcat(dispText, buf);
        printAll(game, dispText);
        if(selectUserTerritory(game->territories, game->users[game->playerTurn].userTerritories, game->users[game->playerTurn].nTerritories, &selectedTerritory, &game->redraws) == 1){
            game->territories[game->users[game->playerTurn].userTerritories[selectedTerritory]].nTroops++;
            gain--;
            game->redraws ^= REDRAW_MAP | REDRAW_BOTTOM;
        }
    }
    while(!(kb_Rising[1] & kb_Mode)){
        kb_scan_rising();
        printAll(game, "Press [mode] to continue");
        if(kb_Rising[6] & kb_Clear){
            if(exitConfirm()){
                gfx_End();
                exit(0);
            }
            game->redraws = REDRAW_ALL;
        }
    }
}

uint8_t genAttackArray(game_t *game, uint8_t *attackArray){
    user_t *thisUser = &game->users[game->playerTurn];
    uint8_t numAttack = 0;
    uint8_t nTerritories = thisUser->nTerritories;

    for(uint8_t i = 0; i < nTerritories; i++){
        territory_t thisTerritory = game->territories[thisUser->userTerritories[i]];
        if(thisTerritory.nTroops > 1){
            uint8_t nConnections = thisTerritory.nConnections;
            for(uint8_t j = 0; j < nConnections; j++){
                if(game->territories[thisTerritory.connIndexes[i]].owner->id != thisUser->id){
                    attackArray[numAttack++] = thisTerritory.id;
                    break;
                }
            }
        }
    }
    return numAttack;
}

void attack(game_t* game){
    //Generate list with territories that can attack
    uint8_t canAttack[game->users[game->playerTurn].nTerritories], nCanAttack = 0;
    int8_t choice = 0;
    nCanAttack = genAttackArray(game, canAttack);
    uint8_t selRetVal = 0;
    
    do{
        selRetVal = selectUserTerritory(game->territories, canAttack, nCanAttack, &choice, &game->redraws);
        if(selRetVal == 1){
            uint8_t toAttack[game->territories[choice].nConnections], nToAttack = 0;
            int8_t attackChoice = 0;
            for(uint8_t i = 0; i < game->territories[choice].nConnections; i++){
                if(game->territories[game->territories[choice].connIndexes[i]].owner->id != game->playerTurn){
                    toAttack[nToAttack++] = game->territories[choice].connIndexes[i];
                }
            }
            
            do{
                selRetVal = selectUserTerritory(game->territories, toAttack, nToAttack, &attackChoice, &game->redraws);
                if(selRetVal == 2){     // Del pressed, cancel attack between territories
                    break;
                }
                if(selRetVal == 1){
                    //Attack between two territories
                }
            }while(game->territories[choice].nTroops > 1);
        }
    }while(!(kb_Rising[1] & kb_Mode) && nCanAttack > 0);
    
}


/*====TERRITORY====*/

void transferTroops(territory_t *from, territory_t *to, int numMove){
    from->nTroops -= numMove;
    to->nTroops += numMove;
}

void gainTerritory(user_t *user, territory_t *territory){
    user->nTerritories++;
    user->userTerritories[user->nTerritories - 1] = territory->id;
    territory->owner = user;
    territory->nTroops = 1;
    gfx_palette[territory->palIndex] = gfx_palette[COLORS_BEFORE_PLAYERS + user->id];
}

void loseTerritory(user_t *user, territory_t *territory){
    int i;
    for(i = 0; i < user->nTerritories; i++){
        if(user->userTerritories[i] == territory->id){
            for(; i < user->nTerritories; i++){
                user->userTerritories[i] = user->userTerritories[i + 1];
            }
            user->nTerritories--;
            return;
        }
    }
}

/*====CONTINENT====*/

bool checkContinent(user_t user, continent_t continent){
    uint8_t i, j;

    for(i = 0; i < continent.nTerritories; i++){
        for(j = 0; j < user.nTerritories; j++){
            if(user.userTerritories[j] == continent.territories[i]){
                break;
            }
            if(user.nTerritories - 1 == j) return false;
        }
    }
    return true;
}

void updateContinentOwnership(continent_t *continents, uint8_t nContinents, user_t *users, uint8_t nUsers){
    uint8_t i;
    for(i = 0; i < nContinents; i++){
        uint8_t j;
        continents[i].owner = NULL;
        for(j = 0; j < nUsers; j++){ //For all users
            if(checkContinent(users[j], continents[i])){
                continents[i].owner = &users[j];
                break;
            }
        }
    }
}

void setOwnedContinent(user_t *user, continent_t *continent){
    continent->owner = user;
}

/*====USER====*/

int userGain(user_t *user){
    uint8_t gain = 3 + (user->nTerritories - 9) / 3;
    return gain;
}