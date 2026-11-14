#include <gtest/gtest.h>

#include "FileMazeSource.h"
#include "MazeError.h"

// These tests read the real maze files shipped in assets/mazes, so they also
// guard against a broken level being committed.

TEST(FileMazeSourceTest, ListsShippedMazesInOrder) {
    FileMazeSource source(COINPATH_MAZE_DIR);
    const auto mazes = source.listMazes();
    ASSERT_GE(mazes.size(), 2u);
    EXPECT_EQ(mazes[0].id, "1-warm-up");
    EXPECT_EQ(mazes[0].displayName, "Warm-up");
}

TEST(FileMazeSourceTest, EveryShippedMazeLoads) {
    FileMazeSource source(COINPATH_MAZE_DIR);
    for (const MazeInfo& info : source.listMazes()) {
        SCOPED_TRACE(info.id);
        EXPECT_NO_THROW(source.loadMaze(info.id));
    }
}

TEST(FileMazeSourceTest, MissingMazeThrows) {
    FileMazeSource source(COINPATH_MAZE_DIR);
    EXPECT_THROW(source.loadMaze("does-not-exist"), MazeError);
}

TEST(FileMazeSourceTest, MissingDirectoryListsNothing) {
    FileMazeSource source("this/directory/does/not/exist");
    EXPECT_TRUE(source.listMazes().empty());
}
