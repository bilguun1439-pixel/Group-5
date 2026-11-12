#include "MazeSelector.h"

MazeSelector::MazeSelector()
    : mazeNames{"Placeholder Maze"} {} // a single empty maze until real layouts are added

const std::vector<std::string>& MazeSelector::getMazeNames() const {
    return mazeNames;
}

Maze MazeSelector::loadMaze(int index) const {
    // TODO: build the real layout for each maze.
    return Maze(mazeNames.at(index), 10, 10, 0, 0);
}
