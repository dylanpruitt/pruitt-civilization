#ifndef DOMAIN_H
#define DOMAIN_H


struct Domain
{
    public:

        std::string name = "Land";

        bool canMoveOnLandTiles = true;

        bool canCoastalEmbark = false;

        bool canCrossOceans = false;

};

#endif // DOMAIN_H
