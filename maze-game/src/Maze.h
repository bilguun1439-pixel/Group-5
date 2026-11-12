#ifndef MAZE_H
#define MAZE_H

#include <string>
#include <vector>

#include "Door.h"

// Maze - one maze layout.
//
// Responsibility: hold the grid of cells, the start position, and the doors
// placed in the maze. Maze answers questions about the layout (e.g. "is this
// cell a wall?") but does not know about the player or the timer.
//
// Future work (not implemented yet):
//  - building real layouts with walls, doors and an exit (FR2)
//  - finding the solution path within a coin budget (FR6)
class Maze {
public:
    // Cell types stored in the grid.
    enum class Cell { Empty, Wall, Door, Exit };

    Maze(const std::string& name, int rows, int cols, int startRow, int startCol);

    const std::string& getName() const;
    int getRows() const;
    int getCols() const;
    int getStartRow() const;
    int getStartCol() const;
    const std::vector<Door>& getDoors() const;

private:
    std::string name;
    std::vector<std::vector<Cell>> grid; // grid[row][col]
    std::vector<Door> doors;
    int startRow;
    int startCol;
};

#endif // MAZE_H
