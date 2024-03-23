
#ifndef BUGS_LIFE_GAME_H
#define BUGS_LIFE_GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include "Grid.hpp"

class Game {
public:
    enum GameState {
        MENU,
        DISPLAY_BUGS,
        DISPLAY_GRID,
        EXIT
    };

    Game();

    void run();

private:
    sf::RenderWindow window;
    sf::Font font;
    std::vector<sf::Text> menuItems;
    Grid grid;
    GameState currentState;

    void initMenu();

    void changeState(GameState newState);

    void render();

    void processEvents();

    void displayBugs();

    void displayGrid();

    std::string directionToString(int direction);

    void loadBugBoard();

    void exitGame();

    void renderMenu();

    void waitForInput();

    void handleKeyPress(sf::Keyboard::Key key);
};
#endif //BUGS_LIFE_GAME_H
