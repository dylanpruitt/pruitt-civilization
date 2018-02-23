#include "WorldMap.h"

WorldMap::WorldMap()
{
    //ctor
}

WorldMap::~WorldMap()
{
    //dtor
}

enum mapTiles : int {

    OCEAN = 0,
    COAST = 1,
    GRASSLAND = 2,
    MOUNTAIN = 3,
    FOREST = 4,
    RUINS = 5,
    SNOW = 6,
    DESERT = 7,

};
