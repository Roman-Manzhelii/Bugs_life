#include "../header/Controls.hpp"
using namespace std;

Controls::Controls() {
    m_rowClicked=-1;
    m_columnClicked=-1;
    m_display = new Display();
    run();
}

Controls::~Controls() {
    delete m_display;
}

void Controls::run() {
    while(m_display->isOpen()) {
        event();
        m_display->getGrid()->update();
        m_display->showGrid();
        changeCellStateByClick();
    }
}

void Controls::event() {
    sf::Event event{};
    while(m_display->getWindow()->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                m_display->getWindow()->close();
                break;
            case sf::Event::KeyPressed:
                pressed(event.key.code);
                break;
            case sf::Event::MouseButtonPressed:
                m_mouseClicked=true;
                break;
            case sf::Event::MouseButtonReleased:
                m_mouseClicked=false;
                    m_columnClicked = -1;
                    m_rowClicked = -1;
                break;
            default:
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
        m_columnClicked = -1;
        m_rowClicked = -1;
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

void Controls::changeCellStateByClick() {
    if (m_mouseClicked) {
        int row = sf::Mouse::getPosition(*m_display->getWindow()).y / (CELL_DEFAULT_SIZE+1);
        int col = sf::Mouse::getPosition(*m_display->getWindow()).x / (CELL_DEFAULT_SIZE+1);
        if (row != m_rowClicked || col != m_columnClicked) {
            m_rowClicked = row;
            m_columnClicked = col;
            m_display->getGrid()->set(row, col, !m_display->getGrid()->get(row, col));
        }
    }
}