#include "types.h"
#include "input.h"
#include "graphics.h"

#include <string.h>

/*====GAME====*/

void resetTimer(){
    timer_Control = TIMER1_DISABLE;
    timer_1_Counter = 32768 / 4;
    timer_1_ReloadValue = 32768 / 4;
}

void draft(game_t *game){
    int gain;
    int8_t selectedTerritory = 0;
    updateContinentOwnership(game->continents, game->nContinents, game->users, game->nUsers);
    gain = userGain(&game->users[game->playerTurn]);
    while(gain > 0){
        char dispText[20] = "Remaining: ", buf[5] = {0};
        printAll(game);
        itoa_custom(buf, gain);
        strcat(dispText, buf);
        printCentered(dispText, MAP_Y_OFFSET + 2 * MAP_HEIGHT + 2);
        gfx_BlitBuffer();
        
        if(selectUserTerritory(game->territories, game->users[game->playerTurn].userTerritories, game->users[game->playerTurn].nTerritories, &selectedTerritory) == -1) continue;
        addTroops(&game->territories[game->users[game->playerTurn].userTerritories[selectedTerritory]], 1);
        gain--;
    }
}


/*====TERRITORY====*/

void addTroops(territory_t *territory, int numAdd){
    territory->nTroops += numAdd;
}

void removeTroops(territory_t *territory, int numRemove){
    territory->nTroops -= numRemove;
}

void transferTroops(territory_t *from, territory_t *to, int numMove){
    removeTroops(from, numMove);
    addTroops(to, numMove);
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
    for(i = 0; i < nContinents; i++){ //For all continents
        uint8_t j;
        setNullContinent(&continents[i]);
        for(j = 0; j < nUsers; j++){ //For all users
            if(checkContinent(users[j], continents[i])){
                setOwnedContinent(&users[j], &continents[i]);
                break;
            }
        }
    }
}

void setOwnedContinent(user_t *user, continent_t *continent){
    continent->owner = user;
}

void setNullContinent(continent_t *continent){
    continent->owner = NULL;
}

/*====USER====*/

int userGain(user_t *user){
    uint8_t gain = 3 + (user->nTerritories - 9) / 3;
    return gain;
}