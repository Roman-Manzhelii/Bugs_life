#include "Game.h"

Game::Game() : window(sf::VideoMode(800, 800), "Bug Game"), grid(10, 10), currentState(MENU), scrollOffset(0) {
    // NOLINTNEXTLINE(cert-msc51-cpp)
    srand(static_cast<unsigned int>(time(nullptr)));
    if (!font.loadFromFile("../OpenSans-SemiboldItalic.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    if (!hopperTexture.loadFromFile("../Hopper.png")) {
        std::cerr << "Failed to load Hopper image\n";
    }
    if (!crawlerTexture.loadFromFile("../Crawler.png")) {
        std::cerr << "Failed to load Crawler image\n";
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
            "4. Display Life History of all Bugs",
            "5. Display all Cells",
            "6. Exit (write Life History of all Bugs to file)"

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
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Game::processEvents() {
    sf::Event event{};

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            // Спершу обробляємо загальні клавіші
            if (event.key.code == sf::Keyboard::Q) {
                changeState(MENU);
            }

            // Тепер обробляємо клавіші, залежно від поточного стану
            if (currentState == DISPLAY_LIFE_HISTORY || currentState == DISPLAY_CELLS) {
                // Додавання прокрутки для історії життя комах
                if (event.key.code == sf::Keyboard::Up) {
                    scrollOffset = std::min(scrollOffset + 30.0f, 0.0f); // Прокрутка вверх
                } else if (event.key.code == sf::Keyboard::Down) {
                    scrollOffset -= 30.0f; // Прокрутка вниз
                }
            } else if (currentState == FIND_BUG && inputActive) {
                // Обробка клавіш вводу ID комахи
                if (ignoreNextTextEntered) {
                    ignoreNextTextEntered = false;
                } else {
                    handleKeyPress(event.key.code);
                }
            } else {
                // Обробка інших клавіш
                handleKeyPress(event.key.code);
            }
        } else if (event.type == sf::Event::TextEntered) {
            if (currentState == FIND_BUG && inputActive) {
                if (ignoreNextTextEntered) {
                    ignoreNextTextEntered = false;
                } else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                    userInput += static_cast<char>(event.text.unicode);
                    inputTextDisplay.setString("Enter Bug ID: " + userInput);
                }
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left && currentState == DISPLAY_GRID) {
                tapBugBoard();
            }
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
        case DISPLAY_LIFE_HISTORY:
            displayLifeHistory();
            break;
        case DISPLAY_CELLS:
            displayCellsAndBugs();
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
                                  directionToString(bug->getDirection()) + " ";

            if (auto hopper = dynamic_cast<Hopper*>(bug.get())) {
                bugInfo += std::to_string(hopper->getHopLength()) + " ";
            }

            bugInfo += (bug->isAlive() ? "Alive" : "Dead");
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

    sf::Text idText;
    idText.setFont(font); // Use the same font loaded for the game
    idText.setCharacterSize(15); // Choose an appropriate size
    idText.setFillColor(sf::Color::White); // White color for visibility

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(j * (CELL_SIZE + CELL_MARGIN), i * (CELL_SIZE + CELL_MARGIN));
            cell.setFillColor(sf::Color::White);
            window.draw(cell);

            auto bugsInCell = grid.getBugsInCell(j, i);
            if (!bugsInCell.empty()) {
                sf::Sprite sprite;

                if (bugsInCell[0]->getType() == "Hopper") {
                    sprite.setTexture(hopperTexture);
                } else if (bugsInCell[0]->getType() == "Crawler") {
                    sprite.setTexture(crawlerTexture);
                }

                sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);
                sprite.setPosition(
                        j * (CELL_SIZE + CELL_MARGIN) + CELL_SIZE / 2,
                        i * (CELL_SIZE + CELL_MARGIN) + CELL_SIZE / 2
                );
                sprite.setScale(
                        (float)CELL_SIZE / sprite.getTexture()->getSize().x,
                        (float)CELL_SIZE / sprite.getTexture()->getSize().y
                );

                // Set the rotation of the sprite based on the direction
                float rotation = 0.0f;
                switch (bugsInCell[0]->getDirection()) {
                    case 1: // North, no rotation needed
                        rotation = 0.0f;
                        break;
                    case 2: // East, rotate 90 degrees
                        rotation = 90.0f;
                        break;
                    case 3: // South, rotate 180 degrees
                        rotation = 180.0f;
                        break;
                    case 4: // West, rotate 270 degrees (or -90 degrees)
                        rotation = 270.0f;
                        break;
                }
                sprite.setRotation(rotation);

                idText.setString(std::to_string(bugsInCell[0]->getId()));
                sf::FloatRect textRect = idText.getLocalBounds();
                idText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                idText.setPosition(sprite.getPosition());

                window.draw(sprite); // Draw the bug sprite first
                window.draw(idText); // Then draw the ID on top
            }
        }
    }
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
    grid.saveLifeHistoryToFile("../bugs_life_history_date_time.out");
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
            changeState(DISPLAY_LIFE_HISTORY);
        } else if (key == sf::Keyboard::Num5) {
            changeState(DISPLAY_CELLS);
        } else if (key == sf::Keyboard::Num6) {
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

void Game::tapBugBoard() {
    for (auto& bug : grid.getBugs()) {
        bug->move();
    }
}

void Game::displayLifeHistory() {

    window.clear(sf::Color::Black); // Очистка вікна

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    float currentY = 50.0f; // Початкова позиція по Y з урахуванням зміщення прокрутки

    for (const auto& bug : grid.getBugs()) {
        std::string baseHistory = "Bug ID: " + std::to_string(bug->getId()) + ", Path: ";
        std::string history;
        std::string segment;

        // Виведення базової інформації
        text.setString(baseHistory);
        text.setPosition(50.0f, currentY + scrollOffset);
        window.draw(text);
        currentY += 30.0f; // Переміщення на наступний рядок

        for (const auto& pos : bug->getPath()) {
            segment = "(" + std::to_string(pos.first) + "," + std::to_string(pos.second) + "), ";
            if (history.length() + segment.length() > 100) {
                text.setString("   " + history);
                text.setPosition(50.0f, currentY + scrollOffset);
                window.draw(text);
                currentY += 30.0f; // Переміщення на наступний рядок
                history = ""; // Очищення історії для наступного сегмента
            }
            history += segment; // Додавання сегмента до історії
        }

// Додаємо статус жука до історії
        segment = (bug->isAlive() ? "Alive!" : "Dead");
        if (history.length() + segment.length() <= 100) {
            history += segment; // Додаємо статус, якщо він влазить у поточний рядок
        } else {
            text.setString("   " + history); // Вивід останньої частини шляху
            text.setPosition(50.0f, currentY + scrollOffset);
            window.draw(text);
            currentY += 30.0f;
            history = segment + " "; // Початок нового рядка зі статусом
        }

        text.setString("   " + history); // Вивід останньої частини історії або тільки статусу
        text.setPosition(50.0f, currentY + scrollOffset);
        window.draw(text);
        currentY += 30.0f;

        if (currentY + scrollOffset > window.getSize().y)
            break;
    }
}


void Game::displayCellsAndBugs() {
    window.clear(sf::Color::Black);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    float currentY = 50.0f;
    std::string line; // Для зберігання інформації про клітинки та жуків

    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            auto bugsInCell = grid.getBugsInCell(j, i);
            std::string cellInfo = "(" + std::to_string(j) + "," + std::to_string(i) + "): ";

            if (bugsInCell.empty()) {
                cellInfo += "empty";
            } else {
                for (auto* bug : bugsInCell) {
                    std::string bugInfo = bug->getType() + " " + std::to_string(bug->getId()) + ", ";
                    if (cellInfo.length() + line.length() + bugInfo.length() > 70) {
                        // Виводимо поточну інформацію
                        text.setString(cellInfo + line);
                        text.setPosition(50.0f, currentY + scrollOffset);
                        window.draw(text);
                        currentY += 30.0f;
                        cellInfo = "       "; // Відступ для подальшого тексту
                        line = bugInfo;
                    } else {
                        line += bugInfo;
                    }
                }
                // Видалення останньої коми
                if (!line.empty()) {
                    line.pop_back();
                    line.pop_back();
                }
            }

            text.setString(cellInfo + line);
            text.setPosition(50.0f, currentY + scrollOffset);
            window.draw(text);
            currentY += 30.0f;
            line.clear(); // Очищення для наступної клітинки

            // Перевірка на перевищення розміру вікна
            if (currentY + scrollOffset > window.getSize().y) break;
        }
        if (currentY + scrollOffset > window.getSize().y) break;
    }
}

