#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include "Bug.h"
#include "Crawler.h"
#include "Hopper.h"
#include "DiagonalBug.h"
#include "SuperBug.h"
#include <memory>

class Grid {
private:
    int m_width, m_height;
    std::vector<std::vector<bool>> m_grid; // Represents the occupied state of grid cells.
    std::vector<std::unique_ptr<Bug>> m_bugs; // Holds all bugs on the grid.

public:
    Grid(int width, int height);
    void loadBugsFromFile(const std::string& filePath); // Loads bugs from a file and populates them on the grid.
    void saveLifeHistoryToFile(const std::string& filePath); // Saves the life history of all bugs to a file.
    const std::vector<std::unique_ptr<Bug>>& getBugs() const; // Returns a constant reference to the vector of unique pointers to bugs.
    std::vector<Bug*> getAllBugsInCell(int x, int y) const; // Returns a vector of copies to all bugs in a specified cell, including dead bugs.
    std::vector<Bug*> getBugsInCell(int x, int y) const;// Returns a vector of copies to living bugs in a specified cell.
    int getWidth() const;
    int getHeight() const;
};

#endif // GRID_HPP
