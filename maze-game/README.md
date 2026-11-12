# Maze Game

A C++ / Qt maze game for CS 3307 Assignment 2. The player moves through a
maze, spends coins to open doors, and tries to reach the exit before a
countdown timer runs out. The full list of functional requirements (FR1-FR8)
and their status is in the repository's top-level `README.md`.

This folder currently holds the **project scaffolding only**: the classes
exist with constructors, fields and getters, but none of the functional
requirements are implemented yet. Running the program opens an empty
"Maze Game" window.

## Requirements

- A C++17 compiler (Apple clang, g++ or MSVC)
- CMake 3.16 or newer
- Qt 6 (Qt 5.15 also works) with the **Widgets** module

Installing Qt: use the official online installer from qt.io (free account
needed) or `brew install qt` on macOS. If CMake can't find Qt, point it at
the install, e.g.
`-DCMAKE_PREFIX_PATH=$HOME/Qt/6.8.0/macos` (adjust the version/path).

## Folder structure

```
maze-game/
├── src/
│   ├── Main.cpp                     Entry point: starts Qt and shows the Game window
│   ├── Game.h / Game.cpp            Game window (QWidget); wires everything together
│   ├── Player.h / Player.cpp        Player position and coin inventory
│   ├── Maze.h / Maze.cpp            One maze layout: grid, start position, doors
│   ├── Door.h / Door.cpp            A door: position, coin value, open state, reward flag
│   ├── GameTimer.h / GameTimer.cpp  Countdown timer
│   └── MazeSelector.h / .cpp        List of available mazes; builds the chosen Maze
├── assets/                          Images or maze layout files (empty for now)
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## Class overview

Only `Main` and `Game` use Qt. The other classes are plain C++, so game rules
can be written and tested without touching the GUI.

| Class | Responsibility | Related FRs |
|-------|----------------|-------------|
| `main` (Main.cpp) | Creates the `QApplication`, shows `Game`, runs the event loop. | — |
| `Game` | Qt window. Owns the maze, player, timer and selector; draws them (`paintEvent`) and handles keys (`keyPressEvent`). | FR1, FR5, FR6 |
| `Player` | Holds the player's row/column and coin count. | FR1, FR3, FR4 |
| `Maze` | Holds the grid of cells (Empty, Wall, Door, Exit) and its doors. | FR2, FR6 |
| `Door` | A door that costs coins, or a reward door that gives coins. | FR3, FR4, FR7 |
| `GameTimer` | Tracks the seconds remaining; `Game` will drive it with a `QTimer`. | FR5, FR6 |
| `MazeSelector` | Lists the available mazes and creates the selected one. | FR8 |

## Build and run

From inside the `maze-game` folder:

```bash
cmake -S . -B build
cmake --build build
./build/MazeGame
```

(On Windows the executable is `build\Debug\MazeGame.exe` or similar.)
The `build/` folder is ignored by Git.

## Run in VS Code

1. Install the **C/C++** and **CMake Tools** extensions (Microsoft).
2. Open the `maze-game` folder itself (File → Open Folder…).
3. When CMake Tools asks, pick a compiler kit (e.g. Clang). If Qt isn't
   found, add `"cmake.configureSettings": { "CMAKE_PREFIX_PATH": "<path to Qt>" }`
   to `.vscode/settings.json`.
4. Use the **Build** and **Run** buttons in the bottom status bar.

You can also open `CMakeLists.txt` directly in **Qt Creator**.

## Working as a pair

Each class has a single responsibility and its own `.h`/`.cpp` pair, so the
work splits cleanly. One suggested split:

- **Developer A:** `Player`, `Door`, `GameTimer` (movement, coins, timer)
- **Developer B:** `Maze`, `MazeSelector` (layouts, pathfinding, selection)

`Game` connects everything, so coordinate before changing it. Work on
separate branches and open pull requests to merge into `main`.
