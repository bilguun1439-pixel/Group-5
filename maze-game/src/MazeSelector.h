#ifndef MAZESELECTOR_H
#define MAZESELECTOR_H

#include <string>
#include <vector>

#include "Maze.h"

// MazeSelector - provides the available maze layouts.
//
// Responsibility: know which mazes exist and create a Maze for the one the
// player picks. Layout data (hard-coded or loaded from assets/) belongs here,
// keeping Maze itself free of any specific layout.
//
// Future work (not implemented yet):
//  - several real layouts and a way for the player to choose one (FR8)
class MazeSelector {
public:
    MazeSelector();

    const std::vector<std::string>& getMazeNames() const;

    // Creates the maze at the given index in getMazeNames().
    Maze loadMaze(int index) const;

private:
    std::vector<std::string> mazeNames;
};

#endif // MAZESELECTOR_H
