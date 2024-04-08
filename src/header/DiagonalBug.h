#ifndef BUGS_LIFE_DIAGONALBUG_H
#define BUGS_LIFE_DIAGONALBUG_H

#include "Bug.h"

class DiagonalBug : public Bug {
public:
    DiagonalBug(int id, std::pair<int, int> position, int direction, int size);

    bool isWayBlocked() const override;
    void move() override;
    std::string getType() const override;
};

#endif //BUGS_LIFE_DIAGONALBUG_H