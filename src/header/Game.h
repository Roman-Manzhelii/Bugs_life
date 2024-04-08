#ifndef BUGS_LIFE_GAME_H
#define BUGS_LIFE_GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Grid.hpp"

// The Game class orchestrates game flow, rendering, and event handling
class Game {
public:
    Game(); // Constructor to initialize game settings
    void run(); // Main game loop

private:
    // Enumeration to represent the current state of the game
    enum GameState {
        MENU,
        DISPLAY_BUGS,
        FIND_BUG,
        DISPLAY_GRID,
        DISPLAY_LIFE_HISTORY,
        DISPLAY_CELLS,
        RUN_SIMULATION,
        EXIT
    } currentState;

    sf::RenderWindow window; // Main game window
    sf::Font font; // Font used for text
    std::vector<sf::Text> menuItems; // Menu items displayed in the main menu
    Grid grid; // Game grid holding all bugs
    std::string searchResult; // Result of bug search
    std::string userInput; // User input for bug search
    bool inputActive = false; // Indicates if input is active
    bool ignoreNextTextEntered = false; // Flag to ignore the next text entered event
    sf::Text inputTextDisplay; // Text display for user input
    sf::Text resultText; // Text display for search results
    sf::Texture hopperTexture; // Texture for Hopper bugs
    sf::Texture crawlerTexture; // Texture for Crawler bugs
    sf::Texture diagonalBugTexture; // Texture for DiagonalBug
    sf::Texture superBugTexture; // Texture for SuperBug
    float scrollOffset; // Offset for scrolling text displays

    void initMenu(); // Initializes the main menu
    void renderMenu(); // Renders the main menu
    void changeState(GameState newState); // Changes the current game state
    void render(); // Renders the current state of the game
    void processEvents(); // Handles all SFML events
    void handleKeyPress(sf::Keyboard::Key key); // Handles key press events
    void loadBugBoard(); // Loads bugs from file to the board
    SuperBug* findSuperBug(); // Finds the SuperBug in the game, if any exist
    static std::string directionToString(int direction); // Converts direction integer to string
    void displayBugs(); // Displays all bugs
    void findBugById(int id); // Finds and displays bug by ID
    void displayGrid(); // Displays the game grid with bugs
    void tapBugBoard(); // Simulates one game tick
    static void resolveFight(std::vector<Bug *> &bugsInCell); // Resolves fights between bugs in the same cell
    void displayLifeHistory(); // Displays the life history of all bugs
    void displayCellsAndBugs(); // Displays all cells and bugs within them
    void exitGame(); // Exits the game
    void runSimulation(); // Runs the game simulation
    bool checkGameOver(); // Checks if the game is over
};
#endif //BUGS_LIFE_GAME_H
