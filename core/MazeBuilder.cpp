#include "MazeBuilder.h"

#include <stdexcept>
#include <utility>

#include "MazeError.h"

namespace {
std::string describe(Position position) {
    return "(" + std::to_string(position.row) + ", " + std::to_string(position.col) + ")";
}
} // namespace

MazeBuilder::MazeBuilder(std::string name, int rows, int cols)
    : name(std::move(name)), rows(rows), cols(cols) {
    if (rows <= 0 || cols <= 0) {
        throw MazeError("A maze needs at least one row and one column");
    }
    grid.assign(rows, std::vector<CellType>(cols, CellType::Floor));
}

MazeBuilder& MazeBuilder::setStart(Position position) {
    requireFreeCell(position, "start");
    start = position;
    return *this;
}

MazeBuilder& MazeBuilder::setExit(Position position) {
    requireFreeCell(position, "exit");
    exit = position;
    return *this;
}

MazeBuilder& MazeBuilder::addWall(Position position) {
    requireFreeCell(position, "wall");
    grid[position.row][position.col] = CellType::Wall;
    return *this;
}

MazeBuilder& MazeBuilder::addPaidDoor(Position position, int cost) {
    requireFreeCell(position, "door");
    try {
        addDoor(std::make_unique<PaidDoor>(position, cost));
    } catch (const std::invalid_argument& error) {
        throw MazeError(std::string(error.what()) + " (door at " + describe(position) + ")");
    }
    return *this;
}

MazeBuilder& MazeBuilder::addRewardDoor(Position position, int reward) {
    requireFreeCell(position, "door");
    try {
        addDoor(std::make_unique<RewardDoor>(position, reward));
    } catch (const std::invalid_argument& error) {
        throw MazeError(std::string(error.what()) + " (door at " + describe(position) + ")");
    }
    return *this;
}

MazeBuilder& MazeBuilder::setStartingCoins(int coins) {
    requireUsable();
    if (coins < 0) {
        throw MazeError("Starting coins cannot be negative");
    }
    startingCoins = coins;
    return *this;
}

MazeBuilder& MazeBuilder::setTimeLimitSeconds(int seconds) {
    requireUsable();
    if (seconds <= 0) {
        throw MazeError("The time limit must be at least one second");
    }
    timeLimitSeconds = seconds;
    return *this;
}

Maze MazeBuilder::build() {
    requireUsable();
    if (!start) {
        throw MazeError("The maze has no start");
    }
    if (!exit) {
        throw MazeError("The maze has no exit");
    }
    if (timeLimitSeconds <= 0) {
        throw MazeError("The maze has no time limit");
    }

    Maze maze(name, std::move(grid), std::move(doors), *start, *exit, startingCoins,
              timeLimitSeconds);
    built = true;

    Graph graph = maze.toGraph();
    if (!graph.isReachable(maze.getCellIndex(*start), maze.getCellIndex(*exit))) {
        throw MazeError("The exit cannot be reached from the start");
    }
    return maze;
}

void MazeBuilder::requireUsable() const {
    if (built) {
        throw std::logic_error("MazeBuilder::build() has already been called");
    }
}

void MazeBuilder::requireFreeCell(Position position, const std::string& what) const {
    requireUsable();
    if (position.row < 0 || position.row >= rows || position.col < 0 || position.col >= cols) {
        throw MazeError("The " + what + " at " + describe(position) + " is outside the maze");
    }
    const bool taken = grid[position.row][position.col] == CellType::Wall ||
                       hasDoorAt(position) || (start && *start == position) ||
                       (exit && *exit == position);
    if (taken) {
        throw MazeError("The " + what + " at " + describe(position) +
                        " overlaps something already placed there");
    }
}

void MazeBuilder::addDoor(std::unique_ptr<Door> door) {
    doors.push_back(std::move(door));
}

bool MazeBuilder::hasDoorAt(Position position) const {
    for (const auto& door : doors) {
        if (door->getPosition() == position) {
            return true;
        }
    }
    return false;
}
