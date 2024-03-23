#include "Game.h"

Game::Game() : window(sf::VideoMode(800, 800), "Bug Game Menu"), grid(10, 10) {
    if (!font.loadFromFile("../OpenSans-SemiboldItalic.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    initMenu();
    loadBugBoard();
    changeState(MENU);
}

void Game::initMenu() {
    std::vector<std::string> texts = {
            "1. Display all Bugs",
            "2. Display Grid",
            "3. Exit (write Life History of all Bugs to file)"
    };

    int posY = 100;
    for (const auto& text : texts) {
        sf::Text menuItem;
        menuItem.setFont(font);
        menuItem.setString(text);
        menuItem.setCharacterSize(24);
        menuItem.setFillColor(sf::Color::White);
        menuItem.setPosition(100, posY);
        menuItems.push_back(menuItem);
        posY += 50;
    }
}

void Game::changeState(GameState newState) {
    currentState = newState;
}

void Game::run() {
    bool gridDisplayed = false;

    while (window.isOpen()) {
        processEvents();

        if (currentState == DISPLAY_GRID && !gridDisplayed) {
            displayGrid();
            gridDisplayed = true;
            waitForInput();
        } else if (currentState != DISPLAY_GRID) {
            render();
            gridDisplayed = false;
        }
    }
}

void Game::waitForInput() {
    sf::Event event;
    while (true) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
                currentState = MENU;
                return;
            }
        }
    }
}


void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            handleKeyPress(event.key.code);
        }
    }
}

void Game::render() {
    window.clear(sf::Color::Black);

    switch (currentState) {
        case MENU:
            renderMenu();
            break;
        case DISPLAY_BUGS:
            displayBugs();
            break;
        case DISPLAY_GRID:
            displayGrid();
            break;
        case EXIT:
            exitGame();
            break;
    }

    window.display();
}

void Game::renderMenu() {
    for (const auto& menuItem : menuItems) {
        window.draw(menuItem);
    }
}

void Game::loadBugBoard() {
    try {
        grid.loadBugsFromFile("../bugs.txt");
        std::cout << "Data successfully loaded from file.\n";
    } catch (const std::exception& e) {
        std::cerr << "Failed to load bugs from file: " << e.what() << "\n";
    }
}

void Game::displayBugs() {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    float currentY = 50.0f;

    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            auto bugs = grid.getBugsInCell(j, i);
            for (auto* bug : bugs) {
                std::string bugInfo = std::to_string(bug->getId()) + " " +
                                      bug->getType() + " (" +
                                      std::to_string(bug->getPosition().first) + "," +
                                      std::to_string(bug->getPosition().second) + ") " +
                                      std::to_string(bug->getSize()) + " " +
                                      directionToString(bug->getDirection()) + " ";


                Hopper* hopper = dynamic_cast<Hopper*>(bug);
                if (hopper) {
                    bugInfo += std::to_string(hopper->getHopLength()) + " ";
                }

                bugInfo += (bug->isAlive() ? "Alive" : "Dead");

                text.setString(bugInfo);
                text.setPosition(50, currentY);
                window.draw(text);

                currentY += 30.0f;
            }
        }
    }
}

void Game::displayGrid() {
    const int GRID_SIZE = 10;
    const int CELL_SIZE = 75;
    const int CELL_MARGIN = 5;

    window.clear(sf::Color::Black);

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(j * (CELL_SIZE + CELL_MARGIN), i * (CELL_SIZE + CELL_MARGIN));
            cell.setFillColor(sf::Color::White);
            window.draw(cell);

            auto bugsInCell = grid.getBugsInCell(j, i);
            if (!bugsInCell.empty()) {
                sf::CircleShape shape(CELL_SIZE / 2 - CELL_MARGIN);
                shape.setPosition(j * (CELL_SIZE + CELL_MARGIN), i * (CELL_SIZE + CELL_MARGIN));

                if (bugsInCell[0]->getType() == "Hopper") {
                    shape.setFillColor(sf::Color::Blue);
                } else if (bugsInCell[0]->getType() == "Crawler") {
                    shape.setFillColor(sf::Color::Red);
                }

                window.draw(shape);
            }
        }
    }

    window.display();
}


std::string Game::directionToString(int direction) {
    switch (direction) {
        case 1: return "North";
        case 2: return "East";
        case 3: return "South";
        case 4: return "West";
        default: return "Unknown";
    }
}


void Game::exitGame() {
    grid.saveLifeHistoryToFile("../life_history.txt");
    window.close();
}

void Game::handleKeyPress(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Q) {
        changeState(MENU);
    } else if (currentState == MENU) {
        switch (key) {
            case sf::Keyboard::Num1:
                changeState(DISPLAY_BUGS);
                break;
            case sf::Keyboard::Num2:
                changeState(DISPLAY_GRID);
                break;
            case sf::Keyboard::Num3:
                changeState(EXIT);
                window.close();
                break;
        }
    }
}