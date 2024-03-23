#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include "Bug.h" // Змініть шлях до вашого файлу Bug.hpp, якщо потрібно
#include "Crawler.h" // Змініть шлях до вашого файлу Crawler.hpp, якщо потрібно
#include "Hopper.h" // Змініть шлях до вашого файлу Hopper.hpp, якщо потрібно
#include <memory> // Для std::unique_ptr

class Grid {
private:
    int m_width, m_height;
    std::vector<std::vector<bool>> m_grid; // Це може бути використано для відображення стану клітин
    std::vector<std::unique_ptr<Bug>> m_bugs; // Вектор вказівників на комах

public:
    Grid(int width, int height);
    void loadBugsFromFile(const std::string& filePath); // Метод для ініціалізації комах з файлу
    void saveLifeHistoryToFile(const std::string& filePath);
    std::string directionToString(int direction);
    std::vector<Bug*> getBugsInCell(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
};

#endif // GRID_HPP
