#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "Grid.hpp"
#include <SFML/Graphics.hpp>

class Display {
public:
    Display();
    void showGrid();
    bool isOpen();

private:
    sf::RenderWindow *m_window;
    Grid *m_grid;
    std::vector<std::vector<sf::RectangleShape*>> m_displayGrid;
    void initWindow();
    void initGrid();
};

#endif
