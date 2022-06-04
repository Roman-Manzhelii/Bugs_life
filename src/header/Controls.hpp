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
    Display* getDisplay();

    private:

    Display* m_display;
    bool m_isRunning;
};

#endif