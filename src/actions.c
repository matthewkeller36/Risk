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

void gainTerritory(user_t* user, territory_t* territory, gfx_sprite_t* map, uint8_t* pal){
    user->nTerritories++;
    user->userTerritories[user->nTerritories - 1] = territory->id;
    territory->owner = user;
    territory->nTroops = 1;
    pal[map->data[territory->x + territory->y * MAP_WIDTH] * 2] = pal[6 + user->id * 2];
    pal[map->data[territory->x + territory->y * MAP_WIDTH] * 2 + 1] = pal[7 + user->id * 2];
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

void updateContinentOwnership(continent_t** continents, uint8_t nContinents, user_t** users, uint8_t nUsers){
    uint8_t i;
    for(i = 0; i < nContinents; i++){ //For all continents
        uint8_t j;
        setNullContinent(continents[i]);
        for(j = 0; j < nUsers; j++){ //For all users
            if(checkContinent(continents[i], users[j])){
                setOwnedContinent(users[j], continents[i]);
                break;
            }
        }
    }
}

void setOwnedContinent(user_t* user, continent_t* continent){
    continent->owner = user;
}

void setNullContinent(continent_t* continent){
    continent->owner = NULL;
}

int userGain(user_t* user, continent_t** continents){
    uint8_t gain = 3 + (user->nTerritories - 9) / 3;
    uint8_t i;
}