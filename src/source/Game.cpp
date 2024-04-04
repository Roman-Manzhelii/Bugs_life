#include "Game.h"
#include <random>

Game::Game() : window(sf::VideoMode(800, 800), "Bug Game"), grid(10, 10), currentState(MENU), scrollOffset(0) {
    // NOLINTNEXTLINE(cert-msc51-cpp)
    srand(static_cast<unsigned int>(time(nullptr))); // Initialize random seed for game mechanics

    // Load fonts and textures for the game, log failures
    if (!font.loadFromFile("../OpenSans-SemiboldItalic.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    if (!hopperTexture.loadFromFile("../Hopper.png")) {
        std::cerr << "Failed to load Hopper image\n";
    }
    if (!crawlerTexture.loadFromFile("../Crawler.png")) {
        std::cerr << "Failed to load Crawler image\n";
    }
    if (!superBugTexture.loadFromFile("../SuperBug.png")) {
        std::cerr << "Failed to load SuperBug image\n";
    }
    initMenu(); // Initialize the main menu
    loadBugBoard(); // Load bugs from file

    // Set up the display for user input
    inputTextDisplay.setFont(font);
    inputTextDisplay.setCharacterSize(24);
    inputTextDisplay.setFillColor(sf::Color::White);
    inputTextDisplay.setPosition(100, 100);
    inputTextDisplay.setString(userInput);

    // Set up the display for search results
    resultText.setFont(font);
    resultText.setCharacterSize(24);
    resultText.setFillColor(sf::Color::Green);
    resultText.setPosition(100, 650);
}

void Game::run() {
    // Main game loop
    while (window.isOpen()) {
        processEvents(); // Process user input and other events
        render(); // Render the current state of the game
    }
}

void Game::initMenu() {
    // Initialize main menu with options
    std::vector<std::string> texts = {
            "1. Display all Bugs",
            "2. Find a Bug",
            "3. Display Grid",
            "4. Display Life History of all Bugs",
            "5. Display all Cells",
            "6. Run Simulation",
            "7. Exit (write Life History of all Bugs to file)"
    };

    float posY = 100.0f; // Initial position for the first menu item
    for (const auto& text : texts) {
        sf::Text menuItem;
        menuItem.setFont(font);
        menuItem.setString(text);
        menuItem.setCharacterSize(24);
        menuItem.setFillColor(sf::Color::White);
        menuItem.setPosition(100, posY);
        menuItems.push_back(menuItem);
        posY += 50; // Increment position for the next item
    }
}

void Game::renderMenu() {
    // Render the main menu
    for (const auto& menuItem : menuItems) {
        window.draw(menuItem);
    }
}

void Game::changeState(GameState newState) {
    currentState = newState; // Change the current game state
}

void Game::render() {
    // Render the game based on the current state
    window.clear(sf::Color::Black); // Clear the window with black background

    switch (currentState) {
        case MENU:
            renderMenu(); // Render the main menu
            break;
        case DISPLAY_BUGS:
            displayBugs(); // Display all bugs
            break;
        case FIND_BUG:
            window.draw(inputTextDisplay); // Display user input for bug search
            if (!inputActive) {
                window.draw(resultText); // Display search results
            }
            break;
        case DISPLAY_GRID:
            displayGrid(); // Display the game grid
            break;
        case DISPLAY_LIFE_HISTORY:
            displayLifeHistory(); // Display life history of all bugs
            break;
        case DISPLAY_CELLS:
            displayCellsAndBugs(); // Display all cells and bugs
            break;
        case RUN_SIMULATION:
            runSimulation(); // Run game simulation
            break;
        case EXIT:
            exitGame(); // Exit the game
            break;
    }
    window.display(); // Display everything drawn to the window
}

void Game::processEvents() {
    sf::Event event{};
    SuperBug* superBug = findSuperBug();

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close(); // Close the window if the close event is triggered
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Q) {
                changeState(MENU); // Return to menu on pressing Q
            }
            // Handle SuperBug direction changes during simulation or when displaying the grid
            if (currentState == RUN_SIMULATION || currentState == DISPLAY_GRID) {
                switch (event.key.code) {
                    case sf::Keyboard::Up:
                        if (superBug) superBug->setDirection(1);
                        break;
                    case sf::Keyboard::Right:
                        if (superBug) superBug->setDirection(2);
                        break;
                    case sf::Keyboard::Down:
                        if (superBug) superBug->setDirection(3);
                        break;
                    case sf::Keyboard::Left:
                        if (superBug) superBug->setDirection(4);
                        break;
                    case sf::Keyboard::Q:
                        changeState(MENU);
                        break;
                    default:
                        break;
                }
            }
            // Handle scrolling in history or cells display
            if (currentState == DISPLAY_LIFE_HISTORY || currentState == DISPLAY_CELLS) {
                if (event.key.code == sf::Keyboard::Up) {
                    scrollOffset = std::min(scrollOffset + 30.0f, 0.0f);
                } else if (event.key.code == sf::Keyboard::Down) {
                    scrollOffset -= 30.0f;
                }
            } else if (currentState == FIND_BUG && inputActive) {
                // Input handling for finding a bug
                if (ignoreNextTextEntered) {
                    ignoreNextTextEntered = false;
                } else {
                    handleKeyPress(event.key.code);
                }
            } else {
                handleKeyPress(event.key.code); // Handle other key presses
            }
        } else if (event.type == sf::Event::TextEntered) {
            if (currentState == FIND_BUG && inputActive) {
                if (ignoreNextTextEntered) {
                    ignoreNextTextEntered = false;
                } else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                    userInput += static_cast<char>(event.text.unicode); // Append typed character to userInput
                    inputTextDisplay.setString("Enter Bug ID: " + userInput); // Update display
                }
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left && currentState == DISPLAY_GRID) {
                tapBugBoard(); // Tap the bug board if mouse is clicked during grid display
            }
        }
    }
}

// Handles key presses and changes game state or performs actions based on the pressed key
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
            changeState(RUN_SIMULATION);
        } else if (key == sf::Keyboard::Num7) {
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

void Game::loadBugBoard() {
    try {
        grid.loadBugsFromFile("../bugs.txt"); // Load bugs from file
        std::cout << "Data successfully loaded from file.\n";
    } catch (const std::exception& e) {
        std::cerr << "Failed to load bugs from file: " << e.what() << "\n";
    }
}

SuperBug* Game::findSuperBug() {
    for (const auto& bug : grid.getBugs()) { // Iterate through all bugs
        if (bug->getType() == "SuperBug") {
            return dynamic_cast<SuperBug*>(bug.get()); // Cast and return SuperBug pointer if found
        }
    }
    return nullptr;
}

// Converts a numeric direction to a string representation
std::string Game::directionToString(int direction) {
    switch (direction) {
        case 1: return "North";
        case 2: return "East";
        case 3: return "South";
        case 4: return "West";
        default: return "Unknown";
    }
}

void Game::displayBugs() {
    sf::Text text;
    text.setFont(font); // Use loaded font
    text.setCharacterSize(24); // Set character size for visibility
    text.setFillColor(sf::Color::White); // Set text color

    float currentY = 50.0f; // Starting Y position for the first bug

    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            auto bugs = grid.getAllBugsInCell(j, i); // Get all bugs in the current cell
            for (auto* bug : bugs) { // Iterate through bugs in the cell
                std::string bugInfo = std::to_string(bug->getId()) + " " +
                                      bug->getType() + " (" +
                                      std::to_string(bug->getPosition().first) + "," +
                                      std::to_string(bug->getPosition().second) + ") " +
                                      std::to_string(bug->getSize()) + " " +
                                      directionToString(bug->getDirection()) + " ";

                // Add hop length if bug is a Hopper
                auto hopper = dynamic_cast<Hopper*>(bug);
                if (hopper) {
                    bugInfo += std::to_string(hopper->getHopLength()) + " ";
                }

                // Indicate whether the bug is alive or dead
                bugInfo += (bug->isAlive() ? "Alive" : "Dead");

                text.setString(bugInfo); // Set the bug info as text string
                text.setPosition(50, currentY); // Position text
                window.draw(text); // Draw text to the window

                currentY += 30.0f; // Increment Y position for the next bug
            }
        }
    }
}

void Game::findBugById(int id) {
    bool found = false;
    for (const auto& bug : grid.getBugs()) {
        if (bug->getId() == id) { // If bug ID matches the search ID
            found = true;
            // Construct the bug's info string
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
    const int GRID_SIZE = 10; // Define grid size
    const int CELL_SIZE = 75; // Define cell size for visuals
    const int CELL_MARGIN = 5; // Define margin between cells

    window.clear(sf::Color::Black); // Clear the window

    sf::Text idText;
    idText.setFont(font); // Use the same font loaded for the game
    idText.setCharacterSize(15); // Choose an appropriate size
    idText.setFillColor(sf::Color::White); // White color for visibility

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE)); // Create a cell shape
            cell.setPosition(j * (CELL_SIZE + CELL_MARGIN), i * (CELL_SIZE + CELL_MARGIN)); // Set cell position
            cell.setFillColor(sf::Color::White); // Set cell color
            window.draw(cell); // Draw the cell

            auto bugsInCell = grid.getBugsInCell(j, i); // Get bugs in the current cell
            if (!bugsInCell.empty()) { // If there are bugs in the cell
                sf::Sprite sprite; // Create a sprite for the bug

                // Assign textures based on bug type
                if (bugsInCell[0]->getType() == "Hopper") {
                    sprite.setTexture(hopperTexture);
                } else if (bugsInCell[0]->getType() == "Crawler") {
                    sprite.setTexture(crawlerTexture);
                } else if (bugsInCell[0]->getType() == "SuperBug") {
                    sprite.setTexture(superBugTexture);
                }

                // Configure and position the sprite
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

void Game::tapBugBoard() {
    // Move all alive bugs
    for (auto& bug : grid.getBugs()) {
        if (bug->isAlive()) {
            bug->move();
        }
    }

    // Check for fights in each cell
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            auto bugsInCell = grid.getBugsInCell(j, i);
            if (bugsInCell.size() > 1) { // If more than one bug in a cell, a fight occurs
                resolveFight(bugsInCell);
            }
        }
    }
}

void Game::resolveFight(std::vector<Bug*>& bugsInCell) {
    Bug* winner = nullptr;
    int maxSize = 0;

    for (auto & i : bugsInCell) {
        if (i->getSize() > maxSize) {
            maxSize = i->getSize();
            winner = i;
        }
    }

    std::vector<Bug*> maxBugs;
    // Determine the largest bug(s)
    for (auto* bug : bugsInCell) {
        if (bug->getSize() == maxSize) {
            maxBugs.push_back(bug);
        }
    }

    if (!maxBugs.empty()) {
        winner = maxBugs[rand() % maxBugs.size()];
    }

    int totalSizeIncrease = 0;
    // Process the aftermath of the fight
    for (auto* bug : bugsInCell) {
        if (bug != winner) {
            bug->setAlive(false); // Losers are marked as dead
            bug->setEatenById(winner->getId()); // Record ID of the bug that ate this one
            totalSizeIncrease += bug->getSize(); // Sum sizes of eaten bugs
        }
    }
    // Increase the size of the winning bug
    if (winner) {
        winner->grow(totalSizeIncrease);
    }
}

void Game::displayLifeHistory() {
    window.clear(sf::Color::Black);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    float currentY = 50.0f;

    // Loop through all bugs to display their life history
    for (const auto& bug : grid.getBugs()) {
        std::string baseHistory = "Bug ID: " + std::to_string(bug->getId()) + ", Path: ";
        std::string history; // To accumulate the path history
        std::string segment; // Temporary variable to hold each segment of the path

        // Displaying base history
        text.setString(baseHistory);
        text.setPosition(50.0f, currentY + scrollOffset);
        window.draw(text);
        currentY += 30.0f; // Move to the next line

        // Loop through the bug's path and add each position to the history
        for (const auto& pos : bug->getPath()) {
            segment = "(" + std::to_string(pos.first) + "," + std::to_string(pos.second) + "), ";
            // Check if adding this segment exceeds the line limit
            if (history.length() + segment.length() > 100) {
                // Display the current history and start a new line
                text.setString("   " + history);
                text.setPosition(50.0f, currentY + scrollOffset);
                window.draw(text);
                currentY += 30.0f;
                history = ""; // Reset history for the next line
            }
            history += segment; // Add the segment to the history
        }

        // Determine if the bug is alive or has been eaten and add this to the history
        if (bug->isAlive()) {
            segment = "Alive!";
        } else {
            segment = "Eaten by " + std::to_string(bug->getEatenById());
        }
        // Check if there's enough space in the current line or if we need a new line
        if (history.length() + segment.length() <= 100) {
            history += segment;
        } else {
            // If not enough space, start a new line
            text.setString("   " + history);
            text.setPosition(50.0f, currentY + scrollOffset);
            window.draw(text);
            currentY += 30.0f; // Move to the next line
            history = segment + " "; // Begin new history with the segment
        }

        text.setString("   " + history);
        text.setPosition(50.0f, currentY + scrollOffset);
        window.draw(text);
        currentY += 30.0f; // Prepare for the next bug's history

        // Break if we've exceeded the window size to prevent drawing off-screen
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

    float currentY = 50.0f; // Initial Y position for the first cell's display
    std::string line;

    // Iterate over each cell in the grid
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            auto bugsInCell = grid.getBugsInCell(j, i); // Retrieve bugs in the current cell
            std::string cellInfo = "(" + std::to_string(j) + "," + std::to_string(i) + "): ";

            if (bugsInCell.empty()) {
                cellInfo += "empty"; // Mark cell as empty if no bugs are present
            } else {
                for (auto* bug : bugsInCell) {
                    std::string bugInfo = bug->getType() + " " + std::to_string(bug->getId()) + ", ";
                    // Check if adding the next bug's info exceeds the line limit
                    if (cellInfo.length() + line.length() + bugInfo.length() > 70) {
                        // Display accumulated bug details and start a new line for additional details
                        text.setString(cellInfo + line);
                        text.setPosition(50.0f, currentY + scrollOffset);
                        window.draw(text);
                        currentY += 30.0f; // Move to the next line
                        cellInfo = "       "; // Indent for continuity in multi-line cells
                        line = bugInfo; // Start accumulating new line of bug details
                    } else {
                        line += bugInfo; // Continue accumulating bug details in the same line
                    }
                }
                // Remove trailing comma and space from the last bug info
                if (!line.empty()) {
                    line.pop_back(); // Remove trailing space
                    line.pop_back(); // Remove trailing comma
                }
            }

            text.setString(cellInfo + line); // Append the final set of bug details to cell info
            text.setPosition(50.0f, currentY + scrollOffset);
            window.draw(text);
            currentY += 30.0f; // Prepare for the next cell's display
            line.clear();
            // Break if we're about to draw beyond the window's viewable area
            if (currentY + scrollOffset > window.getSize().y) break;
        }
        // Also break from the outer loop to avoid unnecessary processing
        if (currentY + scrollOffset > window.getSize().y) break;
    }
}

void Game::exitGame() {
    grid.saveLifeHistoryToFile("../bugs_life_history_date_time.out"); // Save bugs' life history to a file
    window.close(); // Close the game window
}

void Game::runSimulation() {
    bool gameOver = false; // Flag to check if the game is over
    int moveCount = 0; // Counter for the number of moves made

    // Main game loop for the simulation
    while (!gameOver && window.isOpen() && moveCount < 50 && currentState == RUN_SIMULATION) {
        processEvents(); // Process user inputs and events

        // Exit the loop if the state changes away from RUN_SIMULATION
        if (currentState != RUN_SIMULATION) {
            return;
        }

        tapBugBoard(); // Move bugs and handle interactions
        displayGrid(); // Update and display the grid
        window.display(); // Display the updated window

        // Check if the game is over (only one or no bugs alive)
        gameOver = checkGameOver();
        sf::sleep(sf::seconds(1)); // Pause for a moment before the next move

        moveCount++; // Increment move counter
    }

    sf::sleep(sf::seconds(3)); // Wait a bit before exiting to show the final state
    // Only call exitGame if we're still in the simulation state to prevent accidental exits
    if (currentState == RUN_SIMULATION) {
        exitGame();
    }
}

bool Game::checkGameOver() {
    int aliveBugs = 0; // Counter for alive bugs
    for (auto& bug : grid.getBugs()) {
        if (bug->isAlive()) {
            aliveBugs++; // Increment for each alive bug
        }
    }
    return aliveBugs <= 1; // Game is over if one or no bugs are alive
}