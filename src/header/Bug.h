#ifndef BUG_H
#define BUG_H

#include <utility> // for std::pair
#include <list>
#include <string>

class Bug {
protected:
    int id;
    std::pair<int, int> position;
    int direction;
    int size;
    bool alive;
    std::list<std::pair<int, int>> path;

public:
    Bug(int id, std::pair<int, int> position, int direction, int size) : id(id), position(position), direction(direction), size(size), alive(true) {}
    virtual ~Bug() = default;

    virtual void move() = 0;
    bool isWayBlocked() const;
    int getId() const { return id; }
    std::pair<int, int> getPosition() const { return position; }
    int getDirection() const { return direction; }
    int getSize() const { return size; }
    bool isAlive() const { return alive; }
    virtual std::string getType() const = 0;
};

#endif // BUG_H
