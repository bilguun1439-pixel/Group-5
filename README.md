# Coin Path Dash

CS 3307A, Group 5 (Bilguun Purevjav, Ahmed Ali). Assignment 2: Mid-Fidelity Prototype.

Coin Path Dash is a maze game built with C++17 and Qt 6. You choose a maze and steer
the character to the exit with the arrow keys before the countdown runs out. Some
cells are doors. A **paid door** costs coins to open and a **reward door** gives you
coins. You start each maze with a fixed number of coins, so you have to plan a
route you can afford. Under the hood the maze is a weighted graph. Doors are edge
weights, and the solver in Deliverable 3 will search it for the cheapest path to the exit.

**Video walkthrough:** _link to be added_

## Requirements status

| Requirement | Status |
|---|---|
| FR1: Move the character with the arrow keys | Implemented |
| FR2: A maze with doors; reach the exit | Implemented |
| FR3: Start with a set number of coins, spent to open doors | Implemented |
| FR4: Backtrack through an opened door to get the coins back | Planned for Deliverable 3 |
| FR5: Timer showing the time left | Implemented |
| FR6: Show the solution path when time runs out | Planned for Deliverable 3 |
| FR7: Reward doors give coins (and take them back on backtracking) | Partially implemented: reward doors give coins; the reversal comes with FR4 |
| FR8: Choose from several mazes | Implemented |

## Dependencies

- Qt 6 (Widgets module) and Qt Creator, with the course kit
- CMake 3.16 or newer (bundled with Qt Creator)
- A C++17 compiler (MinGW from the Qt installer, MSVC, GCC, or Apple Clang)
- Optional, tests only: GoogleTest 1.15.2. CMake downloads it automatically,
  so the first configure needs an internet connection.

## Build and run

### Qt Creator
1. **File → Open File or Project…** and choose the top-level `CMakeLists.txt`.
2. Select the course Qt 6 kit and click **Configure Project**.
3. Pick the **CoinPathDash** run target and press **Run** (Ctrl+R).

The build copies `assets/mazes/` next to the executable, which is where the game looks
for maze files.

### Command line
```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=<path to Qt>/6.x.x/<kit>
cmake --build build
./build/ui/CoinPathDash        # Windows: build\ui\CoinPathDash.exe
```

## Running the tests
```bash
cmake -S . -B build -DCOINPATH_BUILD_TESTS=ON -DCMAKE_PREFIX_PATH=<path to Qt>
cmake --build build
ctest --test-dir build --output-on-failure
```
In Qt Creator, set `COINPATH_BUILD_TESTS` to `ON` under **Projects → Build → CMake**.
The tests then show up in the **Tests** pane. The test executable links only
`core/`, which shows that the domain builds and runs without Qt.

## How to play
- Arrow keys move you one cell.
- Walking into a closed door opens it if you can pay for it. Brown doors show their
  cost (`-3`) and yellow doors show their reward (`+3`). An opened door stays open
  and is free to walk through again.
- Reach the green **EXIT** before the timer reaches zero.

## Repository structure
```
core/           Domain logic, plain C++ with no Qt (static library coinpath_core)
ui/             Qt Widgets front end (executable CoinPathDash)
tests/          GoogleTest and Google Mock suite for core/
assets/mazes/   Maze layouts, one .maze text file per level
report/         UML, design rationale, Form B, Form C (PDFs)
```

### Main classes
| Module | Class | Responsibility |
|---|---|---|
| core | `GameSession` | One play-through. Applies the movement and coin rules, runs the timer and the game life cycle, and notifies observers. |
| core | `Maze` | The layout: walls, doors, start, exit, and coin and time budget. Converts itself to a `Graph`. |
| core | `Door` → `PaidDoor`, `RewardDoor` | What opening a door does to the wallet (polymorphic). |
| core | `Wallet` | The coin balance; never goes negative. |
| core | `Player` | Position and wallet. |
| core | `GameTimer` | Countdown computed from an injected `IClock`. |
| core | `Graph`, `Node`, `Edge` | Weighted directed graph (adjacency list) with a reachability search. |
| core | `MazeBuilder` | Builds and validates a `Maze` step by step. |
| core | `MazeParser` | Reads the `.maze` text format and drives `MazeBuilder`. |
| core | `IMazeSource` → `FileMazeSource` | Where mazes come from (persistence boundary). |
| core | `IClock` → `SystemClock` | Where time comes from (clock boundary). |
| ui | `MainWindow` | Switches screens and owns the current `GameSession`. Receives `IMazeSource` and `IClock` from `main()`. |
| ui | `MazeSelectView` | Maze list (FR8). |
| ui | `GameView`, `MazeBoard` | Play screen. Sends key presses to the session and redraws when notified. |

### Design patterns
- **Builder (creational):** `MazeBuilder` is the builder, `MazeParser` the director,
  and `Maze` the product. `Maze`'s constructor is private, so every maze in the game
  comes through `MazeBuilder::build()`, which validates it.
- **Observer (behavioural):** `GameSession` is the subject and `IGameObserver` the
  observer interface. `GameView` (and the mock in the tests) are concrete observers.
- **Template Method:** `Door::tryPass()` holds the shared rule about open doors and
  calls `applyOpening()`, which `PaidDoor` and `RewardDoor` override.

### Maze file format
```
name: Warm-up
coins: 1
time: 60
; comment
door a paid 2
door b reward 2
grid:
#######
#S.a.E#
#..b..#
#######
```
`#` wall, `.` floor, `S` start, `E` exit, and a lowercase letter for a door declared above.
A malformed file is rejected with a message that names the line.
