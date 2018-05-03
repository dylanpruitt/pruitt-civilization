#include "MapGenerator.h"
#include "Resource.h"
#include "sharedMethods.h"
#include <iostream>
#include <random>
#include <math.h>

int ForestChanceOutOf100; int MountainBaseChanceOutOf100; int tinyIslandLimit; int tinyIslands = 0;

WorldMap worldmap;
enum mapTiles : int {

    OCEAN = 0,
    COAST = 1,
    GRASSLAND = 2,
    MOUNTAIN = 3,
    FOREST = 4,
    RUINS = 5,
    SNOW = 6,
    DESERT = 7
};

enum resources : int {

    NONE = 0,
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

int islandMaxSize; double sizeScale = 0.81;

MapGenerator::MapGenerator()
{
    //ctor
}

MapGenerator::~MapGenerator()
{
    //dtor
}

void MapGenerator::setMapDefaults (bool isAreaAlreadyUsed[worldSize][worldSize*4]) {

    for (int i = 0; i < worldmap.worldSize; i++){

        for (int j = 0; j < worldmap.worldSize*4; j++){

            worldmap.featureMap[i][j] = mapTiles::OCEAN;

            isAreaAlreadyUsed[i][j] = false;

        }

    }

}

bool MapGenerator::isLandTile (int x, int y) {

    if (worldmap.featureMap[x][y] != mapTiles::COAST && worldmap.featureMap[x][y] != mapTiles::OCEAN) {

        return true;

    } else {

        return false;

    }

}

int MapGenerator::returnLandTiles (int x, int y){

    int LandTiles = 0; /// number of alive tiles

    if (x > 0) {

        if (isLandTile(x-1,y)) {

            LandTiles++;

        }

    }

    if (y > 0) {

        if (isLandTile(x,y-1)) {

            LandTiles++;

        }

    }

    if (x < worldSize-1) {

        if (isLandTile(x+1,y)) {

            LandTiles++;

        }

    }

    if (y < ((worldSize*4)-1)) {

        if (isLandTile(x,y+1)) {

            LandTiles++;

        }

    }

    if (x > 0 && y > 0) {

        if (isLandTile(x-1,y-1)) {

            LandTiles++;

        }

    }

    if (x > 0 && y < ((worldSize*4)-1)) {

        if (isLandTile(x-1,y+1)) {

            LandTiles++;

        }

    }

    if (x < (worldSize-1) && y > 0) {

        if (isLandTile(x+1,y-1)) {

            LandTiles++;

        }

    }

    if (x < (worldSize-1) && y < ((worldSize*4)-1)) {

        if (isLandTile(x+1,y+1)) {

            LandTiles++;

        }

    }

    return LandTiles;
}

int MapGenerator::getSnowTiles (int x, int y) {

    int SnowTiles = 0;

    for (int i = -1; i < 2; i++) {

        for (int j = -1; j < 2; j++) {

            if (i+x >= 0 && j+y >= 0) {

                if (worldmap.featureMap[i+x][j+y] == mapTiles::SNOW) {

                    SnowTiles++;

                }

            }

        }

    }

    return SnowTiles;

}

int MapGenerator::getDesertTiles (int x, int y) {

    int DesertTiles = 0;

    for (int i = -1; i < 2; i++) {

        for (int j = -1; j < 2; j++) {

            if (i+x >= 0 && j+y >= 0) {

                if (worldmap.featureMap[i+x][j+y] == mapTiles::DESERT) {

                    DesertTiles++;

                }

            }

        }

    }

    return DesertTiles;

}

void MapGenerator::generateIsland (double sizeScaleX, double sizeScaleY, int x, int y, int islandArea) {

    for (int i = 0; i < worldSize; i++){

        for (int j = 0; j < worldSize*4; j++){

            int d = sharedMethods::getDistance(x, y, i, y);

            int e = sharedMethods::getDistance(x, y, x, j);

            int tilePlopChance = ((pow(sizeScaleX, d)) * (pow(sizeScaleY, e))) * 100;

            int random = rand() % 99 + 1;

            if (random <= tilePlopChance && d <= islandArea){

                worldmap.featureMap[i][j] = mapTiles::GRASSLAND;

            }

        }

    }

    for (int i = 0; i < worldSize; i++){

        for (int j = 0; j < worldSize*4; j++){

            if (worldmap.featureMap[i][j] == mapTiles::GRASSLAND){

                if (returnLandTiles(i, j) < 2){
                    worldmap.featureMap[i][j] = mapTiles::OCEAN;
                }

            }

            if (worldmap.featureMap[i][j] == mapTiles::OCEAN){

                if (returnLandTiles(i, j) >= 5){
                    worldmap.featureMap[i][j] = mapTiles::GRASSLAND;
                }

            }

        }

    }

}

void MapGenerator::setGenerationParametersByInput (bool inputForIslands) {

    if (inputForIslands == true) {
        std::cout << "How big do you want islands to be (maximum)?\n[1] Tiny\n[2] Small\n[3] Normal\n[4] Big\n[5] Huge" << std::endl;
        islandMaxSize = sharedMethods::bindIntegerInputToRange(1,5,3);
    }

    std::cout << "Forestry?\n[1] Scarce\n[2] Low\n[3] Normal\n[4] Lots" << std::endl;
    ForestChanceOutOf100 = sharedMethods::bindIntegerInputToRange(1,4,3);

    std::cout << "Mountains?\n[1] Few\n[2] Normal\n[3] Lots\n[4] Tons" << std::endl;
    MountainBaseChanceOutOf100 = sharedMethods::bindIntegerInputToRange(1,4,2);

    std::cout << "Tiny Islands?\n[0] No\n[1] Few\n[2] Some\n[3] Normal\n[4] Lots\n[5] Tons" << std::endl;
    tinyIslandLimit = sharedMethods::bindIntegerInputToRange(0,5,3);

}

void MapGenerator::generateRuins () {

    const int ruinChanceOutOf10000 = 400;

    for (int i = 0; i < worldSize; i++) {

        for (int j = 0; j < worldSize*4; j++) {

            if (worldmap.featureMap[i][j] != mapTiles::OCEAN && worldmap.featureMap[i][j] != mapTiles::COAST) {

                int RandomNumber = rand() % 10000 + 1;

                if (RandomNumber <= ruinChanceOutOf10000) {

                    worldmap.featureMap[i][j] = mapTiles::RUINS;

                }

            }

        }

    }

}

void MapGenerator::validateResourceIsInCorrectPlace (int code, int x, int y) {

    if (code == resources::WHALES || code == resources::CRAB || code == resources::PEARLS) {

        if (worldmap.featureMap[x][y] != mapTiles::COAST) {

            code = rand() % 24 + 1;

            validateResourceIsInCorrectPlace(code, x, y);

        }

    }

    if (code != resources::WHALES && code != resources::CRAB && code != resources::PEARLS && code != resources::NONE) {

        if (worldmap.featureMap[x][y] == mapTiles::COAST) {

            code = rand() % 24 + 1;

            validateResourceIsInCorrectPlace(code, x, y);

        }

    }

}

void MapGenerator::generateResources () {

    Resource temp;

    const int resourceChanceOutOf10000 = 750;

    int resourceCode = 0; int resourceAmount = 0;

    for (int i = 0; i < worldSize; i++) {

        for (int j = 0; j < worldSize*4; j++) {

            int x = rand () % 10000 + 1;

            if ( x <= resourceChanceOutOf10000 && worldmap.featureMap[i][j] != mapTiles::OCEAN && worldmap.featureMap[i][j] != mapTiles::MOUNTAIN) {

                int resourceIndex = rand () % 24 + 1;

                resourceCode = resourceIndex;

                validateResourceIsInCorrectPlace (resourceCode, i, j);

                resourceAmount = rand () % 2 + 1;

            } else {

                resourceCode = resources::NONE; resourceAmount = 0;

            }

            temp.ResourceCode = resourceCode;
            temp.AmountOfResource = resourceAmount;

            worldmap.WorldResourceMap[i][j] = temp;

        }

    }

}

void MapGenerator::generateFeatures () {

    for (int i = 0; i < worldSize; i++){

        for (int j = 0; j < worldSize*4; j++){

            if (worldmap.featureMap[i][j] == mapTiles::OCEAN && returnLandTiles(i, j) == 0 && tinyIslands < tinyIslandLimit){

                int random = rand() % 999 + 1;
                int chance = 3;

                if (random <= chance){ generateIsland(0.70, 0.70, i, j, 1); tinyIslands++; }

            }

            if (worldmap.featureMap[i][j] == mapTiles::GRASSLAND && returnLandTiles(i,j) >= 6){

                int random = rand() % 99 + 1;

                if (random <= MountainBaseChanceOutOf100){ worldmap.featureMap[i][j] = mapTiles::MOUNTAIN; }

            }

            if (worldmap.featureMap[i][j] == mapTiles::GRASSLAND && returnLandTiles(i,j) >= 4){

                int random = rand() % 99 + 1;

                if (random <= ForestChanceOutOf100){ worldmap.featureMap[i][j] = mapTiles::FOREST; }

            }

            if (worldmap.featureMap[i][j] == mapTiles::GRASSLAND && returnLandTiles(i,j) >= 7 && sharedMethods::getDistance(i, j, 24, j) <= 8){

                int random = rand() % 99 + 1;

                if (random <= 15*(getDesertTiles(i, j)+1)){ worldmap.featureMap[i][j] = mapTiles::DESERT; }

            }

            if (worldmap.featureMap[i][j] == mapTiles::GRASSLAND && sharedMethods::getDistance(i, j, 24, j) >= 20){

                int random = rand() % 99 + 1;

                if (random <= 15*(getSnowTiles(i, j)+1)){ worldmap.featureMap[i][j] = mapTiles::SNOW; }

            }

        }
    }

    for (int i = 0; i < worldSize; i++){

        for (int j = 0; j < worldSize*4; j++){

            if (worldmap.featureMap[i][j] == mapTiles::OCEAN && returnLandTiles(i,j) >= 1) {
                worldmap.featureMap[i][j] = mapTiles::COAST;
            }

            if (worldmap.featureMap[i][j] == mapTiles::DESERT && getDesertTiles(i,j) == 1) {
                worldmap.featureMap[i][j] = mapTiles::GRASSLAND;
            }

        }
    }

    generateResources();

    generateRuins();

}

void MapGenerator::generateWorld_Continents (WorldMap &mapToGenerateFor) {

    bool isAreaAlreadyUsed[worldSize][worldSize*4];

    setGenerationParametersByInput();

    islandMaxSize*=8;
    ForestChanceOutOf100*=10;
    MountainBaseChanceOutOf100*=10;
    tinyIslandLimit*=8;

    setMapDefaults (isAreaAlreadyUsed);

        int limit = 5;
        int numberOfIslands = rand() % (limit - 4) + 4;

        int x = 0; int y = 0; double xvariance = 0.00, yvariance = 0.00;

        for (int i = 0; i < numberOfIslands; i++){
            xvariance = (0.01) * (rand() % 14);
            yvariance = (0.01) * (rand() % 14);


            bool loop = true; bool canFillArea = false;

            while (loop) {

                x = rand() % (worldSize - 5) + 5;
                y = rand() % (worldSize*4 - 5) + 5;

                int PassedTileChecks = 0; int TotalChecks = 0;

                for (int a = 0; a < worldSize; a++) {

                    for (int b = 0; b < worldSize*4; b++) {

                        if (sharedMethods::getDistance(x,y,a,b) <= islandMaxSize) {
                            TotalChecks++;
                            if (isAreaAlreadyUsed[a][b] == false) {
                                PassedTileChecks++;
                            }
                        }

                    }

                }

                if (PassedTileChecks == TotalChecks) { loop = false; canFillArea = true; }

                if (canFillArea == true) {

                    for (int a = 0; a < worldSize; a++) {

                        for (int b = 0; b < worldSize*4; b++) {

                            if (sharedMethods::getDistance(x,y,a,b) <= islandMaxSize) {
                                isAreaAlreadyUsed[a][b] = true;
                            }
                        }

                    }

                    }

                }

            generateIsland((sizeScale+xvariance), (sizeScale+yvariance+0.02), x, y, islandMaxSize);  std::cout << xvariance << " " << yvariance << std::endl;

        }

        generateFeatures();

        mapToGenerateFor = worldmap;

}

void MapGenerator::generateWorld_Pangaea (WorldMap &mapToGenerateFor) {

    bool isAreaAlreadyUsed[worldSize][worldSize*4];

    setGenerationParametersByInput(false);

    ForestChanceOutOf100*=10;
    MountainBaseChanceOutOf100*=10;
    tinyIslandLimit*=3;

    setMapDefaults (isAreaAlreadyUsed);


    for (int i = 0; i < worldSize; i++){

        for (int j = 0; j < worldSize*4; j++){

            int d = sharedMethods::getDistance((worldSize/2)-1, (worldSize*2)-1, i, (worldSize*2)-1);

            int e = sharedMethods::getDistance(0, (worldSize*2)-1, 0, j);

            int tilePlopChance = ((pow(0.93, d)) * (pow(0.985, e))) * 100;

            int random = rand() % 99 + 1;

            if (random <= tilePlopChance && sharedMethods::getDistance((worldSize/2)-1, (worldSize*2)-1, i, j) < 95){

                worldmap.featureMap[i][j] = mapTiles::GRASSLAND;

            }

        }

    }

    for (int i = 0; i < worldSize; i++){

        for (int j = 0; j < worldSize*4; j++){

            if (worldmap.featureMap[i][j] == mapTiles::GRASSLAND){

                if (returnLandTiles(i, j) < 2){
                    worldmap.featureMap[i][j] = mapTiles::OCEAN;
                }

            }

            if (worldmap.featureMap[i][j] == mapTiles::OCEAN){

                if (returnLandTiles(i, j) >= 5){
                    worldmap.featureMap[i][j] = mapTiles::GRASSLAND;
                }

            }

        }

    }

    generateFeatures();

    mapToGenerateFor = worldmap;

}
