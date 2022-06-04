#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#define CELL_DEFAULT_SIZE 19

#include "Grid.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>

class Display {

    public:

    Display(int width=64, int height=32);
    ~Display();

    void initWindow(int width, int height);

    void initGrid(int width, int height);
    void deleteGrid();
    void showGrid();
    Grid* getGrid();
    sf::RenderWindow* getWindow();

    private:

    int m_win_width;
    int m_win_height;

    
    Grid* m_grid;
    
    sf::RenderWindow* m_window;
    
    std::vector<std::vector<sf::RectangleShape*>> m_displayGrid;
};

#endif