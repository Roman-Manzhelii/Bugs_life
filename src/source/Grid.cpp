#include "Grid.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Grid::Grid(int width, int height) : m_width(width), m_height(height) {
    m_grid = std::vector<std::vector<bool>>(height, std::vector<bool>(width, false));
}

void Grid::loadBugsFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filePath << std::endl;
        return;
    }
    std::string line;

    // Parse each line representing a bug and create an instance of the bug.
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        char type;
        int id, x, y, direction, size, hopLength = 0;
        char delimiter;

        ss >> type >> delimiter
           >> id >> delimiter
           >> x >> delimiter
           >> y >> delimiter
           >> direction >> delimiter
           >> size;

        // Create bug objects based on their type and add them to the grid.
        switch (type) {
            case 'H': // Hopper
                ss >> delimiter >> hopLength;
                m_bugs.push_back(std::make_unique<Hopper>(id, std::make_pair(x, y), direction, size, hopLength));
                break;
            case 'C': // Crawler
                m_bugs.push_back(std::make_unique<Crawler>(id, std::make_pair(x, y), direction, size));
                break;
            case 'D': // DiagonalBug
                m_bugs.push_back(std::make_unique<DiagonalBug>(id, std::make_pair(x, y), direction, size));
                break;
            case 'S': // SuperBug
                m_bugs.push_back(std::make_unique<SuperBug>(id, std::make_pair(x, y), direction, size));
                break;
        }
    }

}

void Grid::saveLifeHistoryToFile(const std::string& filePath) {
    std::ofstream outFile(filePath);
    if (!outFile) {
        std::cerr << "Failed to open " << filePath << " for writing.\n";
        return;
    }

    // Iterate through each bug, recording its path and status.
    for (const auto& bug : m_bugs) {
        outFile << bug->getId() << " "
                << bug->getType() << " Path: ";

        for (const auto& pos : bug->getPath()) {
            outFile << "(" << pos.first << "," << pos.second << "),";
        }

        outFile << (bug->isAlive() ? " Alive!" : " Dead") << "\n";
    }
}

// Returns a reference to the internal container of bugs. This allows read-only access to the bugs managed by the grid.
const std::vector<std::unique_ptr<Bug>>& Grid::getBugs() const {
    return m_bugs;
}

// Retrieves all bugs in a specified cell, both alive and dead. This is useful for debugging or displaying all bugs without status consideration.
std::vector<Bug*> Grid::getAllBugsInCell(int x, int y) const {
    std::vector<Bug*> bugsInCell;
    for (const auto& bug : m_bugs) {
        if (bug->getPosition().first == x && bug->getPosition().second == y) {
            bugsInCell.push_back(bug.get());
        }
    }
    return bugsInCell;
}

// Retrieves only the alive bugs in a specified cell. This is critical for game logic that operates on active bugs.
std::vector<Bug*> Grid::getBugsInCell(int x, int y) const {
    std::vector<Bug*> aliveBugsInCell;
    for (const auto& bug : m_bugs) {
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
            std::cerr << "Coordinates out of bounds" << std::endl;
            return {}; // Early return if the coordinates are out of the grid bounds.
        }
        if (bug->getPosition().first == x && bug->getPosition().second == y && bug->isAlive()) {
            aliveBugsInCell.push_back(bug.get());
        }
    }
    return aliveBugsInCell;
}


int Grid::getWidth() const { return m_width; }
int Grid::getHeight() const { return m_height; }