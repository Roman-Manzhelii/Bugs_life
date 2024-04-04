#include "Bug.h"

Bug::Bug(int id, std::pair<int, int> position, int direction, int size) : id(id), position(position), direction(direction), size(size), alive(true) {
    path.push_back(position); // Initial position is added to the bug's path
}

bool Bug::isWayBlocked() const {
    const int MAX_X = 9, MAX_Y = 9; // Assumes a 10x10 grid with indices 0-9

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
            return false; // Default case should not be reached
    }
}

void Bug::grow(int sizeIncrease) {
    this->size += sizeIncrease;
}

int Bug::getId() const { return id; }
std::pair<int, int> Bug::getPosition() const { return position; }
int Bug::getDirection() const { return direction; }
int Bug::getSize() const { return size; }
const std::list<std::pair<int, int>> &Bug::getPath() const { return path; }
bool Bug::isAlive() const { return alive; }
void Bug::setAlive(bool a) { alive = a; }
void Bug::setEatenById(int ID) { eatenById = ID; }
int Bug::getEatenById() const { return eatenById; }