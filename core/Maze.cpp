#include "Maze.h"

#include <utility>

Maze::Maze(std::string name, std::vector<std::vector<CellType>> grid,
           std::vector<std::unique_ptr<Door>> doors, Position start, Position exit,
           int startingCoins, int timeLimitSeconds)
    : name(std::move(name)),
      grid(std::move(grid)),
      doors(std::move(doors)),
      start(start),
      exit(exit),
      startingCoins(startingCoins),
      timeLimitSeconds(timeLimitSeconds) {}

const std::string& Maze::getName() const {
    return name;
}

int Maze::getRows() const {
    return static_cast<int>(grid.size());
}

int Maze::getCols() const {
    return grid.empty() ? 0 : static_cast<int>(grid.front().size());
}

Position Maze::getStart() const {
    return start;
}

Position Maze::getExit() const {
    return exit;
}

int Maze::getStartingCoins() const {
    return startingCoins;
}

int Maze::getTimeLimitSeconds() const {
    return timeLimitSeconds;
}

bool Maze::isInside(Position position) const {
    return position.row >= 0 && position.row < getRows() && position.col >= 0 &&
           position.col < getCols();
}

bool Maze::isWall(Position position) const {
    return !isInside(position) || grid[position.row][position.col] == CellType::Wall;
}

bool Maze::isExit(Position position) const {
    return position == exit;
}

Door* Maze::doorAt(Position position) {
    for (const auto& door : doors) {
        if (door->getPosition() == position) {
            return door.get();
        }
    }
    return nullptr;
}

const Door* Maze::doorAt(Position position) const {
    for (const auto& door : doors) {
        if (door->getPosition() == position) {
            return door.get();
        }
    }
    return nullptr;
}

int Maze::getCellIndex(Position position) const {
    return position.row * getCols() + position.col;
}

Graph Maze::toGraph() const {
    Graph graph(getRows() * getCols());
    const Direction directions[] = {Direction::Up, Direction::Down, Direction::Left,
                                    Direction::Right};

    for (int row = 0; row < getRows(); ++row) {
        for (int col = 0; col < getCols(); ++col) {
            const Position from{row, col};
            if (isWall(from)) {
                continue;
            }
            for (Direction direction : directions) {
                const Position to = step(from, direction);
                if (isWall(to)) {
                    continue;
                }
                const Door* door = doorAt(to);
                const double cost = door ? -door->getCoinChange() : 0.0;
                graph.insertEdge(getCellIndex(from), getCellIndex(to), cost,
                                 door ? "door" : "floor");
            }
        }
    }
    return graph;
}
