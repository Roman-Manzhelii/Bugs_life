cmake_minimum_required(VERSION 3.27)
project(Bugs_life)

set(CMAKE_CXX_STANDARD 14)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR H:/SFML/lib/cmake/SFML)

# Знайти пакет SFML за допомогою заданих компонентів
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

# Включити директорію заголовочних файлів SFML
include_directories(H:/SFML/include)

# Додати виконуваний файл вашого проекту
add_executable(Bugs_life
        src/header/Grid.hpp
        src/source/Grid.cpp
        src/source/main.cpp
        src/source/Bug.cpp
        src/header/Bug.h
        src/source/Crawler.cpp
        src/header/Crawler.h
        src/source/Hopper.cpp
        src/header/Hopper.h
        src/header/Hopper.h
        src/header/Game.h
        src/source/Game.cpp
        src/source/Game.cpp
        src/source/SuperBug.cpp
        src/header/SuperBug.h)

# Лінкувати бібліотеки SFML з вашим проектом
target_link_libraries(Bugs_life sfml-system sfml-window sfml-graphics sfml-audio)

# Це не обов'язково, якщо ви не маєте додаткових заголовочних файлів у директорії окрім SFML
include_directories(src/header)
