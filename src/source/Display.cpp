#include "../header/Display.hpp"
#include "SFML/Graphics.hpp"
#include <cassert>
#include <stdexcept>

using namespace std;

Display::Display(int width, int height) {
    initWindow(width, height);
    initGrid(width, height);
}

Display::~Display() {
    delete m_window;
    deleteGrid();
    delete m_grid;
}


void Display::initWindow(int width, int height) {
    // CELL_DEFAULT_SIZE +1 to make a margin between cells
    m_win_width = (CELL_DEFAULT_SIZE+1) * width;
    m_win_height = (CELL_DEFAULT_SIZE+1) * height;

    m_window = new sf::RenderWindow(sf::VideoMode(m_win_width, m_win_height), "Conways' game of life");
    m_window->setFramerateLimit(0);
    m_window->setVerticalSyncEnabled(true);
}


void Display::initGrid(int width, int height) {
    m_grid = new Grid(width, height);
    for (int i = 0; i < height; i++) {
        m_displayGrid.push_back(vector<sf::RectangleShape*>());
        for (int j = 0; j < width; j++) {
            sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(CELL_DEFAULT_SIZE, CELL_DEFAULT_SIZE));
            m_displayGrid[i].push_back(rect);
            m_displayGrid[i][j]->setFillColor(sf::Color::White);
            m_displayGrid[i][j]->setPosition(sf::Vector2f(j*(CELL_DEFAULT_SIZE+1), i*(CELL_DEFAULT_SIZE+1)));
        }
    }
}


void Display::showGrid() {
    m_window->clear();
    for (int i = 0; i < m_displayGrid.size(); i++) 
        for (int j = 0; j < m_displayGrid[i].size(); j++) 
            if (!m_grid->get(i, j)) m_window->draw(*m_displayGrid[i][j]);
    m_window->display();
}

void Display::deleteGrid() {
    for (int i = 0; i < m_grid->getHeight(); i++) {
        for (auto ptr : m_displayGrid[i]) 
            delete ptr;
        m_displayGrid[i].clear();
    } m_displayGrid.clear();
}

Grid* Display::getGrid() {
    return m_grid;
}

sf::RenderWindow* Display::getWindow() {
    return m_window;
}


