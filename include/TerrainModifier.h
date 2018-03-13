#ifndef TERRAINMODIFIER_H
#define TERRAINMODIFIER_H


class TerrainModifier
{
    public:
        TerrainModifier() {}
        TerrainModifier(double attackmodifier, double defensemodifier) {
            attackModifier = attackmodifier;
            defenseModifier = defensemodifier;
        }
        virtual ~TerrainModifier() {}

        double attackModifier;
        double defenseModifier;

    protected:
    private:
};

#endif // TERRAINMODIFIER_H