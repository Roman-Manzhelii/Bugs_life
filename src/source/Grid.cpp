#include "Grid.hpp"
#include <chrono>
#include <ctime>
#include <cassert>
#include <stdexcept>

using namespace std;

Grid::Grid(int height, int width) : m_height(height), m_width(width) {
    initialize();
}

bool Grid::isRowEmpty(int row) {
    if (row < 0 || row > m_height)
        throw std::out_of_range("define a good index");
    for (bool value : m_grid[row])
        if (value)
            return false;
    return true;
}

bool Grid::isEmpty() {
    for (int i = 0; i < m_height; i++)
        if (!isRowEmpty(i))
            return false;
    return true; 
}

void Grid::initialize() {
    for (int i = 0; i < m_height; i++) {
        m_grid.push_back(vector<bool>());
        for (int j = 0; j < m_width; j++)
            m_grid[i].push_back(false);
    }
}

void Grid::reset() {
    for (int i = 0; i < m_height; i++)
        for (int j = 0; j < m_width; j++)
            m_grid[i][j] = false;
}

void Grid::reverse() {
    for (int i = 0; i < m_height; i++) 
        for (int j = 0; j < m_width; j++)
            m_grid[i][j] = !m_grid[i][j];
}

void Grid::random() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < m_height; i++)
        for (int j = 0; j < m_width; j++) {
            auto finish = std::chrono::high_resolution_clock::now();
            int chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
            m_grid[i][j] = chrono % 2 == 0;
        }

}

bool Grid::get(int row, int col) {
    return m_grid[row][col];
}

void Grid::next() {

}