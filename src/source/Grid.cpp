#include "../header/Grid.hpp"
#include <chrono>
#include <ctime>
#include <cassert>
#include <stdexcept>

using namespace std;

Grid::Grid(int width, int height) : m_width(width), m_height(height) {
    m_isLaunched = false;
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
    srand(time(NULL));
    for (int i = 0; i < m_height; i++)
        for (int j = 0; j < m_width; j++) {
            m_grid[i][j] = rand() % 2;
        }

}

bool Grid::get(int row, int col) {
    return m_grid[row][col];
}

void Grid::next() {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<bool>> grid = copy();
    for (int row=0; row < m_height; row++)
        for (int col=0; col < m_width; col++)
            updateCell(row, col, grid);
}

void Grid::updateCell(int row, int col, std::vector<std::vector<bool>> grid) {
    int neighbors=0;
    for (int i=-1; i<2;i++)
        for (int j=-1;j<2;j++)
            if (j != 0 || i != 0) 
                if (inRange(row+i, col+j))
                    if(grid[row+i][col+j])
                        neighbors++;
    if (neighbors == 3) m_grid[row][col] = true;
    else if (neighbors != 2) m_grid[row][col] = false;
}

bool Grid::inRange(int row, int col) {
    return row >= 0 && row < m_height && col >= 0 && col < m_width;
}

void Grid::stop() {
    m_isLaunched = false;
}

void Grid::launch() {
    m_isLaunched = true;
}

void Grid::update() {
    if (m_isLaunched) next();
}

int Grid::getWidth() {
    return m_width;
}

int Grid::getHeight() {
    return m_height;
}

bool Grid::launched() {
    return m_isLaunched;
}

vector<vector<bool>> Grid::copy() {
    vector<vector<bool>> newGrid;
    for (vector<bool> v : m_grid)
        newGrid.push_back(vector<bool>(v.begin(), v.end()));
    return newGrid;
}