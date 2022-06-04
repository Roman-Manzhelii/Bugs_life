#include "../header/Controls.hpp"
#include <chrono>
using namespace std;

Controls::Controls() {

    m_display = new Display();
    run();
}

Controls::~Controls() {
    delete m_display;
}

void Controls::run() {
    while(m_display->getWindow()->isOpen()) {
        auto start = std::chrono::high_resolution_clock::now();
        event();
        m_display->getGrid()->update();
        m_display->showGrid();
        auto end = std::chrono::high_resolution_clock::now();
        double chrono = (std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()) / 1000.0;
        cout << "FPS: " << 1/chrono << endl;
    }
}

void Controls::event() {
    sf::Event event;
    while(m_display->getWindow()->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_display->getWindow()->close();
                break;
            case sf::Event::KeyPressed:
                pressed(event.key.code);
                break;
        } 
    }
}

void Controls::pressed(sf::Keyboard::Key key) {
    switch (key) {
    case sf::Keyboard::Right:
        if (!(m_display->getGrid()->launched())) m_display->getGrid()->next();
        break;

    case sf::Keyboard::R:
        if (!m_display->getGrid()->launched())m_display->getGrid()->random();
        break;

    case sf::Keyboard::E:
        cout << !m_display->getGrid()->launched() << endl;
        if(!m_display->getGrid()->launched()) 
            m_display->getGrid()->reverse();
        break;

    case sf::Keyboard::C:
        if(!m_display->getGrid()->launched())m_display->getGrid()->reset();
        break;

    case sf::Keyboard::Space:
        m_display->getGrid()->launch();
        break;

    case sf::Keyboard::Escape:
        m_display->getGrid()->stop();
        break;

    default:
        break;
    }
}