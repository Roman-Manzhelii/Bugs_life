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
        src/header/Controls.hpp
        src/header/Display.hpp
        src/header/Grid.hpp
        src/source/Controls.cpp
        src/source/Display.cpp
        src/source/Grid.cpp
        src/source/main.cpp)

# Лінкувати бібліотеки SFML з вашим проектом
target_link_libraries(Bugs_life sfml-system sfml-window sfml-graphics sfml-audio)

# Це не обов'язково, якщо ви не маєте додаткових заголовочних файлів у директорії окрім SFML
include_directories(src/header)
