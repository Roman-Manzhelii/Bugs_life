#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include "../header/Display.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>

class Controls {

    public:

    Controls();
    ~Controls();

    void run();
    void event();
    void pressed(sf::Keyboard::Key key);
    void changeCellStateByClick();
    Display* getDisplay();

    private:

    Display* m_display;
    bool m_isRunning;
    bool m_mouseClicked=false;
    int m_rowClicked;
    int m_columnClicked;
};

#endif