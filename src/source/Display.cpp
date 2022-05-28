#include "Display.hpp"
#include <cassert>
#include <stdexcept>

using namespace std;

Display::Display(int width, int height) : m_grid(Grid(width, height)) {
    initWindow(width, height);
    initGrid(width, height);
}

Display::~Display() {
    delete m_window;
    deleteGrid();
}

void Display::initWindow(int width, int height) {
    // CELL_DEFAULT_SIZE +1 to make a margin between cells
    m_win_width = (CELL_DEFAULT_SIZE+1) * width;
    m_win_height = (CELL_DEFAULT_SIZE+1) * height;

    m_window = new sf::RenderWindow(sf::VideoMode(m_win_width, m_win_height), "Conways' game of life");

}

void Display::initGrid(int width, int height) {
    for (int i = 0; i < width; i++) {
        m_displayGrid.push_back(vector<sf::RectangleShape*>());
        for (int j = 0; j < height; j++) {
            m_displayGrid[i].push_back(new sf::RectangleShape(sf::Vector2f(CELL_DEFAULT_SIZE, CELL_DEFAULT_SIZE)));
            m_displayGrid[i][j]->setFillColor(sf::Color::White);
            m_displayGrid[i][j]->setPosition(sf::Vector2f(i*(CELL_DEFAULT_SIZE+1), j*(CELL_DEFAULT_SIZE+1)));
        }
    }
}

void Display::showGrid() {
    for (int i = 0; i < m_grid.getWidth(); i++) 
        for (int j = 0; j <m_grid.getHeight(); j++)
            if (m_grid.get(i, j)) m_displayGrid[i][j]->setFillColor(sf::Color::Black);
            else m_displayGrid[i][j]->setFillColor(sf::Color::White);
}

void Display::deleteGrid() {
    for (int i = 0; i < m_grid.getWidth(); i++) 
        for (int j = 0; j <m_grid.getHeight(); j++)
            delete m_displayGrid[i][j];
}


