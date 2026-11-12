cmake_minimum_required(VERSION 3.16)

project(MazeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt's meta-object compiler (needed for classes with Q_OBJECT, e.g. Game).
set(CMAKE_AUTOMOC ON)

# Works with either Qt 6 or Qt 5, whichever is installed.
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Add new .h/.cpp files here as the project grows.
add_executable(MazeGame
    src/Main.cpp
    src/Game.h          src/Game.cpp
    src/Player.h        src/Player.cpp
    src/Maze.h          src/Maze.cpp
    src/Door.h          src/Door.cpp
    src/GameTimer.h     src/GameTimer.cpp
    src/MazeSelector.h  src/MazeSelector.cpp
)

target_link_libraries(MazeGame PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(MazeGame PRIVATE -Wall -Wextra -Wpedantic)
endif()
