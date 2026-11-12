#include "Maze.h"

Maze::Maze(const std::string& name, int rows, int cols, int startRow, int startCol)
    : name(name),
      grid(rows, std::vector<Cell>(cols, Cell::Empty)), // every cell starts Empty
      doors(),
      startRow(startRow),
      startCol(startCol) {}

const std::string& Maze::getName() const {
    return name;
}

int Maze::getRows() const {
    return static_cast<int>(grid.size());
}

int Maze::getCols() const {
    return grid.empty() ? 0 : static_cast<int>(grid[0].size());
}

int Maze::getStartRow() const {
    return startRow;
}

int Maze::getStartCol() const {
    return startCol;
}

const std::vector<Door>& Maze::getDoors() const {
    return doors;
}
