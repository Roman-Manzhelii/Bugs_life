// SuperBug.cpp
#include "SuperBug.h"

SuperBug::SuperBug(int id, std::pair<int, int> position, int direction, int size)
        : Bug(id, position, direction, size) {}

void SuperBug::move() {
    // Choose a new direction randomly until it's not blocked
    while (isWayBlocked()) {
        direction = rand() % 4 + 1; // Random direction between 1 and 4
    }

    // Move one unit in the chosen direction
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

    // Record the new position in the crawler's path history
    path.push_back(position);
}

void SuperBug::setDirection(int newDirection) {
    direction = newDirection;
}

std::string SuperBug::getType() const  { return "SuperBug"; }