#ifndef POSITION_H
#define POSITION_H


class Position
{
    public:
        Position();
        virtual ~Position();

        int x;
        int y;

        void setCoordinates (int x, int y) {
            this->x = x;
            this->y = y;
        }
    protected:
    private:
};

#endif // POSITION_H
