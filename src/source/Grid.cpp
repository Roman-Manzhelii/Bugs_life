#include "Grid.hpp"

Grid::Grid(int width, int height) : m_width(width), m_height(height) {
    m_grid = std::vector<std::vector<bool>>(height, std::vector<bool>(width, false));
}

bool Grid::get(int row, int col) const {
    return m_grid[row][col];
}
