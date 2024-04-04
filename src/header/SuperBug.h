
#ifndef BUGS_LIFE_SUPERBUG_H
#define BUGS_LIFE_SUPERBUG_H

#include "Bug.h"

class SuperBug : public Bug {
public:
    SuperBug(int id, std::pair<int, int> position, int direction, int size);
    void move() override;
    std::string getType() const override;
    void setDirection(int newDirection);// Allows changing the direction of the SuperBug
};
#endif //BUGS_LIFE_SUPERBUG_H