#ifndef ISLAND_H
#define ISLAND_H


class island
{
    public:
        island() {}
        virtual ~island() {}

        int minXbound, minYbound, maxXbound, maxYbound;

        int areaSize = 5;

    protected:
    private:
};

#endif // ISLAND_H
