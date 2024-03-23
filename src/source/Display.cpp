#include "Display.hpp"
#include <SFML/Graphics.hpp>

constexpr int GRID_SIZE = 10;
constexpr int CELL_MARGIN = 1;
int CELL_DEFAULT_SIZE = 20;

Display::Display() {
    initWindow();
    initGrid();
}

void Display::initWindow() {
    int targetWindowSize = 960;
    int cellPlusMarginSize = targetWindowSize / GRID_SIZE;
    int cellSize = cellPlusMarginSize - CELL_MARGIN;

    m_window = new sf::RenderWindow(sf::VideoMode(targetWindowSize, targetWindowSize), "10x10 Grid Display");
    CELL_DEFAULT_SIZE = cellSize;
}

void Display::initGrid() {
    m_grid = new Grid(GRID_SIZE, GRID_SIZE);

    for (int i = 0; i < GRID_SIZE; ++i) {
        std::vector<sf::RectangleShape*> row;
        for (int j = 0; j < GRID_SIZE; ++j) {
            auto* rect = new sf::RectangleShape(sf::Vector2f(CELL_DEFAULT_SIZE, CELL_DEFAULT_SIZE));
            rect->setFillColor(sf::Color::White);
            rect->setPosition(j * (CELL_DEFAULT_SIZE + CELL_MARGIN), i * (CELL_DEFAULT_SIZE + CELL_MARGIN));
            row.push_back(rect);
        }
        m_displayGrid.push_back(row);
    }
}

void Display::showGrid() {
    if (!m_window->isOpen()) return;

    sf::Event event;
    while (m_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) m_window->close();
    }

    m_window->clear(sf::Color::Black); // Clear with black background
    for (const auto& row : m_displayGrid) {
        for (const auto& cell : row) {
            m_window->draw(*cell);
        }
    }
    m_window->display();
}

bool Display::isOpen() {
    return m_window->isOpen();
}
