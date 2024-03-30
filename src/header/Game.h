
#ifndef BUGS_LIFE_GAME_H
#define BUGS_LIFE_GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include "Grid.hpp"

class Game {
public:
    Game();
    void run();

private:
    enum GameState {
        MENU,
        DISPLAY_BUGS,
        FIND_BUG,
        DISPLAY_GRID,
        DISPLAY_LIFE_HISTORY,
        DISPLAY_CELLS,
        EXIT
    } currentState;

    sf::RenderWindow window;
    sf::Font font;
    std::vector<sf::Text> menuItems;
    Grid grid;
    std::string searchResult;
    std::string userInput;
    bool inputActive = false;
    bool ignoreNextTextEntered = false;
    sf::Text inputTextDisplay;
    sf::Text resultText;
    sf::Texture hopperTexture;
    sf::Texture crawlerTexture;
    float scrollOffset;

    void initMenu();

    void changeState(GameState newState);

    void render();

    void processEvents();

    void displayBugs();

    void findBugById(int id);

    void displayGrid();

    static std::string directionToString(int direction);

    void loadBugBoard();

    void exitGame();

    void renderMenu();

    void handleKeyPress(sf::Keyboard::Key key);

    void tapBugBoard();

    void displayLifeHistory();

    void displayCellsAndBugs();
};
#endif //BUGS_LIFE_GAME_H
