#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include "Bug.h"
#include "Crawler.h"
#include "Hopper.h"
#include <memory>

class Grid {
private:
    int m_width, m_height;
    std::vector<std::vector<bool>> m_grid;
    std::vector<std::unique_ptr<Bug>> m_bugs;

public:
    Grid(int width, int height);
    void loadBugsFromFile(const std::string& filePath);
    void saveLifeHistoryToFile(const std::string& filePath);
    const std::vector<std::unique_ptr<Bug>>& getBugs() const;
    static std::string directionToString(int direction);
    std::vector<Bug*> getBugsInCell(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
};

#endif // GRID_HPP
