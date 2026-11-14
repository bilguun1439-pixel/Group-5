#include <gtest/gtest.h>

#include "MazeBuilder.h"
#include "MazeError.h"

namespace {
// A 1x5 corridor: S . door . E
MazeBuilder corridor() {
    MazeBuilder builder("Corridor", 1, 5);
    builder.setStart({0, 0}).setExit({0, 4}).setTimeLimitSeconds(30).setStartingCoins(2);
    return builder;
}
} // namespace

TEST(MazeBuilderTest, BuildsAValidMaze) {
    MazeBuilder builder = corridor();
    builder.addPaidDoor({0, 2}, 2);
    Maze maze = builder.build();

    EXPECT_EQ(maze.getName(), "Corridor");
    EXPECT_EQ(maze.getRows(), 1);
    EXPECT_EQ(maze.getCols(), 5);
    EXPECT_EQ(maze.getStartingCoins(), 2);
    EXPECT_TRUE(maze.isExit({0, 4}));
    ASSERT_NE(maze.doorAt({0, 2}), nullptr);
    EXPECT_EQ(maze.doorAt({0, 2})->getCoinChange(), -2);
    EXPECT_EQ(maze.doorAt({0, 1}), nullptr);
}

TEST(MazeBuilderTest, OutsideCellsCountAsWalls) {
    Maze maze = corridor().build();
    EXPECT_TRUE(maze.isWall({-1, 0}));
    EXPECT_TRUE(maze.isWall({0, 5}));
    EXPECT_FALSE(maze.isWall({0, 1}));
}

TEST(MazeBuilderTest, RejectsUnreachableExit) {
    MazeBuilder builder = corridor();
    builder.addWall({0, 2});
    EXPECT_THROW(builder.build(), MazeError);
}

TEST(MazeBuilderTest, RejectsMissingStartOrExit) {
    MazeBuilder noStart("x", 1, 3);
    noStart.setExit({0, 2}).setTimeLimitSeconds(10);
    EXPECT_THROW(noStart.build(), MazeError);

    MazeBuilder noExit("x", 1, 3);
    noExit.setStart({0, 0}).setTimeLimitSeconds(10);
    EXPECT_THROW(noExit.build(), MazeError);
}

TEST(MazeBuilderTest, RejectsOverlapsAndOutOfBoundsCells) {
    MazeBuilder builder = corridor();
    EXPECT_THROW(builder.addWall({0, 0}), MazeError);      // the start is there
    EXPECT_THROW(builder.addPaidDoor({0, 9}, 1), MazeError); // outside
    builder.addWall({0, 1});
    EXPECT_THROW(builder.addRewardDoor({0, 1}, 1), MazeError);
}

TEST(MazeBuilderTest, RejectsBadSettings) {
    EXPECT_THROW(MazeBuilder("x", 0, 3), MazeError);
    MazeBuilder builder("x", 1, 3);
    EXPECT_THROW(builder.setStartingCoins(-1), MazeError);
    EXPECT_THROW(builder.setTimeLimitSeconds(0), MazeError);
    EXPECT_THROW(builder.addPaidDoor({0, 1}, 0), MazeError);
}

TEST(MazeBuilderTest, GraphWeightsEdgesIntoDoorsByCost) {
    MazeBuilder builder = corridor();
    builder.addRewardDoor({0, 2}, 3);
    Maze maze = builder.build();
    Graph graph = maze.toGraph();

    const Edge* intoDoor = graph.getEdgeBetween(maze.getCellIndex({0, 1}), maze.getCellIndex({0, 2}));
    ASSERT_NE(intoDoor, nullptr);
    EXPECT_DOUBLE_EQ(intoDoor->getWeight(), -3.0);
    EXPECT_EQ(intoDoor->getType(), "door");
}
