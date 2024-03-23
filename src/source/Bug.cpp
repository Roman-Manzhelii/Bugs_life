#include "Bug.h"

bool Bug::isWayBlocked() const {
    const int MAX_X = 9, MAX_Y = 9;

    switch (direction) {
        case 1: // North
            return position.second == 0;
        case 2: // East
            return position.first == MAX_X;
        case 3: // South
            return position.second == MAX_Y;
        case 4: // West
            return position.first == 0;
        default:
            return false;
    }
}

