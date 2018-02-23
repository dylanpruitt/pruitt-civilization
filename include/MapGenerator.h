#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include "WorldMap.h"

class MapGenerator
{
    public:

        MapGenerator();
        virtual ~MapGenerator();

        void generateWorld_Continents(WorldMap &mapToGenerateFor);

        void generateWorld_Pangaea(WorldMap &mapToGenerateFor);


    protected:

    private:

        static const int worldSize = 50;

        int getDistance (int x, int y, int x2, int y2);

        int getDesertTiles (int x, int y);

        int getSnowTiles (int x, int y);

        void setMapDefaults(bool isAreaAlreadyUsed[worldSize][worldSize*4]);

        bool isLandTile(int x, int y);

        int returnLandTiles(int x, int y);

        void generateIsland(double sizeScale, int x, int y, int islandArea = 5);

        void setGenerationParametersByInput (bool inputForIslands = true);

        void generateRuins();

        void validateResourceIsInCorrectPlace(int code, int x, int y);

        void generateResources();

        void generateFeatures();

};

#endif // MAPGENERATOR_H
