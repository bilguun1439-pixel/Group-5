#ifndef COINPATH_MAZE_H
#define COINPATH_MAZE_H

#include <memory>
#include <string>
#include <vector>

#include "Door.h"
#include "Graph.h"
#include "Position.h"

enum class CellType { Floor, Wall };

// Maze - one maze layout (FR2): the grid of walls and floor, the doors, the
// start and exit, and the coin and time budget the level starts with.
//
// Maze answers questions about the layout ("is this cell a wall?", "is there
// a door here?") but knows nothing about the player or the timer. Mazes are
// created only by MazeBuilder, which guarantees every Maze is valid.
class Maze {
public:
    Maze(Maze&&) noexcept = default;
    Maze& operator=(Maze&&) noexcept = default;
    Maze(const Maze&) = delete;
    Maze& operator=(const Maze&) = delete;
    ~Maze() = default;

    const std::string& getName() const;
    int getRows() const;
    int getCols() const;
    Position getStart() const;
    Position getExit() const;
    int getStartingCoins() const;
    int getTimeLimitSeconds() const;

    bool isInside(Position position) const;
    bool isWall(Position position) const; // cells outside the maze count as walls
    bool isExit(Position position) const;

    // The door in this cell, or nullptr if there is none. Not owning.
    Door* doorAt(Position position);
    const Door* doorAt(Position position) const;

    // Node number of a cell in toGraph(): cells are numbered row by row.
    int getCellIndex(Position position) const;

    // The maze as a graph: one node per cell, and an edge between every pair
    // of neighbouring non-wall cells. An edge into a door cell is weighted by
    // the coins it costs (negative for a reward door).
    Graph toGraph() const;

private:
    friend class MazeBuilder;

    Maze(std::string name, std::vector<std::vector<CellType>> grid,
         std::vector<std::unique_ptr<Door>> doors, Position start, Position exit,
         int startingCoins, int timeLimitSeconds);

    std::string name;
    std::vector<std::vector<CellType>> grid; // grid[row][col]
    std::vector<std::unique_ptr<Door>> doors;
    Position start;
    Position exit;
    int startingCoins;
    int timeLimitSeconds;
};

#endif // COINPATH_MAZE_H
