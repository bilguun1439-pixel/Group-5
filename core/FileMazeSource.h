#ifndef COINPATH_FILEMAZESOURCE_H
#define COINPATH_FILEMAZESOURCE_H

#include <filesystem>

#include "IMazeSource.h"

// FileMazeSource - loads mazes from the *.maze files in one directory.
// A maze's id is its file name without the extension.
class FileMazeSource final : public IMazeSource {
public:
    explicit FileMazeSource(std::filesystem::path directory);

    // Sorted by id. Empty if the directory is missing.
    std::vector<MazeInfo> listMazes() const override;
    Maze loadMaze(const std::string& id) const override;

private:
    std::filesystem::path pathFor(const std::string& id) const;

    std::filesystem::path directory;
};

#endif // COINPATH_FILEMAZESOURCE_H
