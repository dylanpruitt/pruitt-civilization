#ifndef WORLDMAP_H
#define WORLDMAP_H
#include "Resource.h"

class WorldMap
{
    public:
        WorldMap();
        virtual ~WorldMap();

        static const int worldSize = 50;

        int featureMap[worldSize][worldSize*4];

        Resource WorldResourceMap[worldSize][worldSize*4];

        int WorldTerritoryMap[worldSize][worldSize*4];

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

        enum resources : int {

            NONE,
            HORSES,
            IRON,
            RUBBER,
            COPPER,
            PEARLS,
            CRAB,
            WHALES,
            SALT,
            SPICES,
            STONE,
            MARBLE,
            COCOA,
            SHEEP,
            CATTLE,
            DIAMONDS,
            NUTMEG,
            GINGER,
            SILK,
            DYES,
            CITRUS,
            IVORY,
            FURS,
            SILVER,
            GOLD,

        };

    protected:
    private:
};

#endif // WORLDMAP_H
