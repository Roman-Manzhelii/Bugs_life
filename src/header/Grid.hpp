#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <vector>

class Grid {

    public:

    /**
     * @brief Construct a new Grid object.
     * define the width and the height of the grid.
     * 
     * @param width width of the grid
     * @param height height of the grid 
     */
    Grid(int width=20, int height=15);

    /**
     * @brief check if the grid is empty
     * 
     * @return true if no row is empty
     * @return false if a row or many rows is (are) empty.
     */
    bool isEmpty();

    /**
     * @brief 
     * 
     * @param row 
     * @return true 
     * @return false 
     */
    bool isRowEmpty(int row);

    /**
    * @brief 
    * 
    */
    void next();

    /**
     * @brief 
     * 
     */
    void random();

    /**
     * @brief 
     * 
     */
    void reverse();

    /**
     * @brief 
     * 
     */
    void reset();

    /**
     * @brief initializes the grid.
     * 
     * places booleans in grid and set 
     * theirs default value to false.
     * 
     */
    void initialize();

    /**
     * @brief Get the Width object
     * 
     * @return int 
     */
    int getWidth();

    /**
     * @brief Get the Height object
     * 
     * @return int 
     */
    int getHeight();

    bool get(int row, int col);

    private:

    const int m_width;
    const int m_height;
    std::vector<std::vector<bool>> m_grid;

};

#endif