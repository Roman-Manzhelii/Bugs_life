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

        if (type == 'H') {
            ss >> delimiter >> hopLength;
            m_bugs.push_back(std::make_unique<Hopper>(id, std::make_pair(x, y), direction, size, hopLength));
        } else if (type == 'C') {
            m_bugs.push_back(std::make_unique<Crawler>(id, std::make_pair(x, y), direction, size));
        }
    }

}

void Grid::saveLifeHistoryToFile(const std::string& filePath) {
    std::ofstream outFile(filePath);
    if (!outFile) {
        std::cerr << "Failed to open " << filePath << " for writing.\n";
        return;
    }

    for (const auto& bug : m_bugs) {
        outFile << bug->getId() << " "
                << bug->getType() << " ("
                << bug->getPosition().first << ","
                << bug->getPosition().second << ") "
                << bug->getSize() << " "
                << directionToString(bug->getDirection()) << " "
                << (bug->isAlive() ? "Alive" : "Dead") << "\n";
    }
}

const std::vector<std::unique_ptr<Bug>>& Grid::getBugs() const {
    return m_bugs;
}

std::vector<Bug*> Grid::getBugsInCell(int x, int y) const {
    std::vector<Bug*> bugsInCell;
    for (const auto& bug : m_bugs) {
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
            std::cerr << "Coordinates out of bounds" << std::endl;
            return {};
        }
        if (bug->getPosition().first == x && bug->getPosition().second == y) {
            bugsInCell.push_back(bug.get());
        }
    }
    return bugsInCell;
}

std::string Grid::directionToString(int direction) {
    switch (direction) {
        case 1: return "North";
        case 2: return "East";
        case 3: return "South";
        case 4: return "West";
        default: return "Unknown";
    }
}

int Grid::getWidth() const { return m_width; }
int Grid::getHeight() const { return m_height; }