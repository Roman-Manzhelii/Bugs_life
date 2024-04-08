#include "Hopper.h"
#include <cstdlib>

Hopper::Hopper(int id, std::pair<int, int> position, int direction, int size, int hopLength)
        : Bug(id, position, direction, size), hopLength(hopLength) {
}

void Hopper::move() {
    // If the way is blocked, choose a new direction randomly until it's not blocked
    while (isWayBlocked()) {
        direction = rand() % 4 + 1; // Random direction between 1 and 4
    }

    // Attempt to move hopLength units in the chosen direction
    for (int i = 0; i < hopLength; ++i) {
        if (!isWayBlocked()) { // Check if the hop can be completed
            switch (direction) {
                case 1: // North
                    position.second -= 1;
                    break;
                case 2: // East
                    position.first += 1;
                    break;
                case 3: // South
                    position.second += 1;
                    break;
                case 4: // West
                    position.first -= 1;
                    break;
            }

        } else {
            // If the hop cannot be completed because of hitting the edge, stop the hop
            break;
        }
    }

    // Record the new position in the Hopper's path history
    path.push_back(position);
}

std::string Hopper::getType() const{ return "Hopper"; }

int Hopper::getHopLength() const {
    return hopLength;
}

