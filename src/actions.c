#include "types.h"

/*====GAME====*/

void addTroops(territory_t* territory, int numAdd){
    territory->nTroops += numAdd;
}

void removeTroops(territory_t* territory, int numRemove){
    territory->nTroops -= numRemove;
}

void transferTroops(territory_t* from, territory_t* to, int numMove){
    removeTroops(from, numMove);
    addTroops(to, numMove);
}

/*====TERRITORY====*/

void gainTerritory(user_t* user, territory_t* territory){
    user->nTerritories++;
    user->userTerritories[user->nTerritories - 1] = territory->id;
    territory->owner = user;
}

void loseTerritory(user_t* user, territory_t* territory){
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

bool checkContinent(user_t* user, continent_t* continent){
    uint8_t i, j;

    for(i = 0; i < continent->nTerritories; i++){
        for(j = 0; j < user->nTerritories; j++){
            if(user->userTerritories[j] == continent->territories[i]){
                break;
            }
            if(user->nTerritories - 1 == j) return false;
        }
    }
    return true;
}

void gainContinent(user_t* user, continent_t* continent){
    if(checkContinent(user, continent)){
        user->continents[user->nContinents] = continent->id;
        user->nContinents++;
        continent->owner = user;
    }
}

void loseContinent(user_t* user, continent_t* continent){
    if(!checkContinent(user, continent)){
        int i;
        for(i = 0; i < user->nContinents; i++){
            if(user->continents[i] == continent->id){
                for(; i < user->nContinents; i++){
                    user->continents[i] = user->continents[i + 1];
                }
                user->nContinents--;
                return;
            }
        }
    }
}