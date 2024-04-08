#ifndef BUG_H
#define BUG_H

#include <utility>
#include <list>
#include <string>

// Base class for all bug types in the game
class Bug {
protected:
    int id; // Unique identifier for the bug
    std::pair<int, int> position; // Current position of the bug on the grid
    int direction; // Current direction of movement
    int size; // Size of the bug, affects interactions with other bugs
    bool alive; // Alive status of the bug
    int eatenById = -1; // ID of the bug that ate this bug, -1 if not applicable
    std::list<std::pair<int, int>> path; // List of positions the bug has visited

public:
    // Constructor initializing bug properties
    Bug(int id, std::pair<int, int> position, int direction, int size);
    virtual ~Bug() = default; // Virtual destructor for polymorphic use

    // Pure virtual function for moving the bug, to be implemented by derived classes
    virtual void move() = 0;

    // Checks if the bug's way is blocked, to be implemented based on game logic
    virtual bool isWayBlocked() const;

    // Getter methods for bug properties
    int getId() const;
    std::pair<int, int> getPosition() const;
    int getDirection() const;
    int getSize() const;
    const std::list<std::pair<int, int>>& getPath() const;
    bool isAlive() const;
    void setAlive(bool a);
    void grow(int sizeIncrease); // Increases the size of the bug
    void setEatenById(int ID); // Sets the ID of the bug that ate this bug
    int getEatenById() const; // Gets the ID of the bug that ate this bug

    // Pure virtual function to get the type of the bug, to be implemented by derived classes
    virtual std::string getType() const = 0;
};

#endif // BUG_H
