#include "Display.hpp"
#include "Bug.h"
#include "Crawler.h"
#include "Hopper.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
    Display display;
    std::vector<Bug*> bugs; // Vector of pointers to Bug objects

    std::ifstream file("../bugs.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open bugs.txt" << std::endl;
        return 1;
    }
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        char type;
        int id, x, y, direction, size, hopLength = 0;
        char delimiter;

        ss >> type >> delimiter
           >> id >> delimiter
           >> x >> delimiter
           >> y >> delimiter
           >> direction >> delimiter
           >> size;

        if (type == 'H') {
            ss >> delimiter >> hopLength;
            bugs.push_back(new Hopper(id, {x, y}, direction, size, hopLength));
        } else if (type == 'C') {
            bugs.push_back(new Crawler(id, {x, y}, direction, size));
        }
    }


    for (Bug* bug : bugs) {
        std::cout << "Type: " << bug->getType() << "\t"
                  << "ID: " << bug->getId() << "\t"
                  << "Position: (" << bug->getPosition().first << ", " << bug->getPosition().second << ")\t"
                  << "Direction: " << bug->getDirection() << "\t"
                  << "Size: " << bug->getSize();

        Hopper* hopper = dynamic_cast<Hopper*>(bug);
        if (hopper) {
            std::cout << "\tHop Length: " << hopper->getHopLength();
        }
        std::cout << std::endl;
    }

    while (display.isOpen()) {
        display.showGrid();
    }

    for (Bug* bug : bugs) {
        delete bug;
    }
    bugs.clear();

    return 0;
}
