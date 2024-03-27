#include "Game.h"

Game::Game() : window(sf::VideoMode(800, 800), "Bug Game"), grid(10, 10), currentState(MENU) {
    if (!font.loadFromFile("../OpenSans-SemiboldItalic.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    initMenu();
    loadBugBoard();

    inputTextDisplay.setFont(font);
    inputTextDisplay.setCharacterSize(24);
    inputTextDisplay.setFillColor(sf::Color::White);
    inputTextDisplay.setPosition(100, 100);
    inputTextDisplay.setString(userInput);

    resultText.setFont(font);
    resultText.setCharacterSize(24);
    resultText.setFillColor(sf::Color::Green);
    resultText.setPosition(100, 650);
}

void Game::initMenu() {
    std::vector<std::string> texts = {
            "1. Display all Bugs",
            "2. Find a Bug",
            "3. Display Grid",
            "4. Exit (write Life History of all Bugs to file)"
    };

    float posY = 100.0f;
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
    bool gridDisplayed;

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
    sf::Event event{};
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
    sf::Event event{};


    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (currentState == FIND_BUG && inputActive && ignoreNextTextEntered) {
                    ignoreNextTextEntered = false;
                    continue;
                }
                handleKeyPress(event.key.code);
                break;
            case sf::Event::TextEntered:
                if (currentState == FIND_BUG && inputActive) {
                    if (ignoreNextTextEntered) {
                        ignoreNextTextEntered = false;
                        continue;
                    }
                    if (event.text.unicode >= 32 && event.text.unicode < 128) {
                            userInput += static_cast<char>(event.text.unicode);
                            inputTextDisplay.setString("Enter Bug ID: " + userInput);
                        }
                }
                break;
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
        case FIND_BUG:
            window.draw(inputTextDisplay);
            if (!inputActive) {
                window.draw(resultText);
            }
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


                auto hopper = dynamic_cast<Hopper*>(bug);
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

void Game::findBugById(int id) {
    bool found = false;
    for (const auto& bug : grid.getBugs()) {
        if (bug->getId() == id) {
            found = true;
            std::string bugInfo = std::to_string(bug->getId()) + " " + bug->getType() + " (" +
                                  std::to_string(bug->getPosition().first) + "," +
                                  std::to_string(bug->getPosition().second) + ") " +
                                  std::to_string(bug->getSize()) + " " +
                                  directionToString(bug->getDirection()) + " " +
                                  (bug->isAlive() ? "Alive" : "Dead");

            if (auto hopper = dynamic_cast<Hopper*>(bug.get())) {
                bugInfo += " " + std::to_string(hopper->getHopLength());
            }

            resultText.setString(bugInfo);
            break;
        }
    }

    if (!found) {
        resultText.setString("Bug with ID " + std::to_string(id) + " not found.");
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
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
                sf::CircleShape shape(float(CELL_SIZE) / 2 - CELL_MARGIN);
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
#pragma clang diagnostic pop

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
    if (currentState == MENU) {
        if (key == sf::Keyboard::Num1) {
            changeState(DISPLAY_BUGS);
        } else if (key == sf::Keyboard::Num2) {
            userInput.clear();
            inputActive = true;
            ignoreNextTextEntered = true;
            inputTextDisplay.setString("Enter Bug ID: ");
            changeState(FIND_BUG);
        } else if (key == sf::Keyboard::Num3) {
            changeState(DISPLAY_GRID);
        } else if (key == sf::Keyboard::Num4) {
            changeState(EXIT);
        }
    } else if (currentState == FIND_BUG) {
        if (key == sf::Keyboard::Enter) {
            if (userInput.empty()) {
                resultText.setString("No ID entered. Press 'Q' to return.");
            } else {
                try {
                    int id = std::stoi(userInput);
                    if (id < 0 || id > 99999) {
                        throw std::out_of_range("The ID number is out of range.");
                    }
                    findBugById(id);
                } catch (const std::invalid_argument&) {
                    resultText.setString("Invalid ID format. Press 'Q' to retry.");
                } catch (const std::out_of_range&) {
                    resultText.setString("ID is out of range. Press 'Q' to retry.");
                }
            }
            userInput.clear();
            inputTextDisplay.setString("Enter Bug ID: ");
            inputActive = false;
        } else if (key == sf::Keyboard::BackSpace && !userInput.empty()) {
            userInput.pop_back();
            inputTextDisplay.setString("Enter Bug ID: " + userInput);
        } else if (key == sf::Keyboard::Q) {
            changeState(MENU);
            userInput.clear();
            inputTextDisplay.setString("");
            resultText.setString("");
        }
    } else if (key == sf::Keyboard::Q) {
        changeState(MENU);
    }
}