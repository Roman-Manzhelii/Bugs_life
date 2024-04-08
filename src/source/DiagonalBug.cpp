#include "DiagonalBug.h"
#include <cstdlib>

DiagonalBug::DiagonalBug(int id, std::pair<int, int> position, int direction, int size)
        : Bug(id, position, direction, size) {
}
bool DiagonalBug::isWayBlocked() const {
    const int MAX_X = 9, MAX_Y = 9; // Assumes a 10x10 grid with indices 0-9

    switch (direction) {
        case 1: // North-East
            return (position.first == MAX_X) || (position.second == 0);
        case 2: // South-East
            return (position.first == MAX_X) || (position.second == MAX_Y);
        case 3: // South-West
            return (position.first == 0) || (position.second == MAX_Y);
        case 4: // North-West
            return (position.first == 0) || (position.second == 0);
        default:
            return false; // Default case should not be reached
    }
}
void DiagonalBug::move() {
    // Choose a new direction randomly until it's not blocked
    while (isWayBlocked()) {
        direction = rand() % 4 + 1; // Random direction between 1 and 4
    }

    // Move one unit in the chosen direction
    switch (direction) {
        case 1: // North-East
            position.first += 1;
            position.second -= 1;
            break;
        case 2: // South-East
            position.first += 1;
            position.second += 1;
            break;
        case 3: // South-West
            position.first -= 1;
            position.second += 1;
            break;
        case 4: // North-West
            position.first -= 1;
            position.second -= 1;
            break;
    }

    // Record the new position in the crawler's path history
    path.push_back(position);
}

std::string DiagonalBug::getType() const  { return "DiagonalBug"; }