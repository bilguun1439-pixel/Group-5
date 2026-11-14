#ifndef COINPATH_MAZEBUILDER_H
#define COINPATH_MAZEBUILDER_H

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Door.h"
#include "Maze.h"
#include "Position.h"

// MazeBuilder - the Builder in the Builder pattern (creational).
//
// A Maze has many parts (grid, walls, several kinds of door, start, exit,
// coin and time budget) that must be consistent with each other. Building it
// step by step lets a director (MazeParser) assemble those parts in any
// order, while build() checks the whole maze once before any Maze exists.
// Maze's constructor is private, so every Maze in the program is valid.
//
// Each step throws MazeError for a cell outside the grid or a cell that is
// already taken. A builder produces one maze; it cannot be used after build().
class MazeBuilder {
public:
    // Throws MazeError unless both dimensions are positive.
    MazeBuilder(std::string name, int rows, int cols);

    MazeBuilder& setStart(Position position);
    MazeBuilder& setExit(Position position);
    MazeBuilder& addWall(Position position);
    MazeBuilder& addPaidDoor(Position position, int cost);
    MazeBuilder& addRewardDoor(Position position, int reward);
    MazeBuilder& setStartingCoins(int coins);
    MazeBuilder& setTimeLimitSeconds(int seconds);

    // Validates the maze and returns it. Throws MazeError if the start or
    // exit is missing, the time limit is not positive, or the exit cannot be
    // reached from the start.
    Maze build();

private:
    void requireUsable() const;
    void requireFreeCell(Position position, const std::string& what) const;
    void addDoor(std::unique_ptr<Door> door);
    bool hasDoorAt(Position position) const;

    std::string name;
    int rows;
    int cols;
    std::vector<std::vector<CellType>> grid;
    std::vector<std::unique_ptr<Door>> doors;
    std::optional<Position> start;
    std::optional<Position> exit;
    int startingCoins = 0;
    int timeLimitSeconds = 0;
    bool built = false;
};

#endif // COINPATH_MAZEBUILDER_H
