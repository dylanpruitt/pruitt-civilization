#ifndef PLANT_H
#define PLANT_H


class Plant
{
    public:
        Plant() {}
        virtual ~Plant() {}

        int xPos;
        int yPos;

        std::string Name;

        int growTime;
        int timeAlive;

    protected:
    private:
};

#endif // PLANT_H
