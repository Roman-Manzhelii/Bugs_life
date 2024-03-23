#ifndef GRID_HPP
#define GRID_HPP

#include <vector>

class Grid {
public:
    Grid(int width, int height);
    bool get(int row, int col) const;

private:
    std::vector<std::vector<bool>> m_grid;
    int m_width, m_height;
};

#endif
