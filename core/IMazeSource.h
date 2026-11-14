#ifndef COINPATH_IMAZESOURCE_H
#define COINPATH_IMAZESOURCE_H

#include <string>
#include <vector>

#include "Maze.h"

// Summary of one available maze, for the selection screen (FR8).
struct MazeInfo {
    std::string id;          // passed back to loadMaze()
    std::string displayName; // shown to the player
};

// IMazeSource - where mazes come from (the persistence boundary of DP7).
//
// The game asks for mazes through this interface and never opens files
// itself, so the storage can change (files, resources, a test double)
// without touching the domain. FileMazeSource is the real implementation.
class IMazeSource {
public:
    virtual ~IMazeSource() = default;

    virtual std::vector<MazeInfo> listMazes() const = 0;

    // Throws MazeError if the maze is missing, unreadable, or invalid.
    virtual Maze loadMaze(const std::string& id) const = 0;
};

#endif // COINPATH_IMAZESOURCE_H
