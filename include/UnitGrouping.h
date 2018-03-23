#ifndef UNITGROUPING_H
#define UNITGROUPING_H
#include <vector>
#include <string>

class UnitGrouping
{
    public:
        UnitGrouping();
        virtual ~UnitGrouping();

        std::string name;

        std::vector<int> memberUnitIndices;

        int rgbValue[3] = {128, 128, 128};
    protected:
    private:
};

#endif // UNITGROUPING_H

/**

Unit Groupings help players to organize their units into more manageable groups.

**/
