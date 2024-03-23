#ifndef HOPPER_H
#define HOPPER_H

#include "Bug.h"

class Hopper : public Bug {
private:
    int hopLength;

public:
    Hopper(int id, std::pair<int, int> position, int direction, int size, int hopLength);

    void move() override;
    std::string getType() const override;

    int getHopLength() const;
};

#endif // HOPPER_H
