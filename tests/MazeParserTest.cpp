#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "MazeError.h"
#include "MazeParser.h"

namespace {
Maze parse(const std::string& text) {
    std::istringstream input(text);
    return MazeParser().parse(input);
}

const char* const kValid =
    "name: Tiny\n"
    "coins: 3\n"
    "; comment lines are ignored\n"
    "time: 45\n"
    "door a paid 2\n"
    "door b reward 1\n"
    "grid:\n"
    "#######\n"
    "#S.a.E#\n"
    "#..b..#\n"
    "#######\n";

// Replaces the first occurrence of `from` in the valid maze with `to`.
std::string variant(const std::string& from, const std::string& to) {
    std::string text = kValid;
    text.replace(text.find(from), from.size(), to);
    return text;
}
} // namespace

TEST(MazeParserTest, ParsesHeaderAndGrid) {
    Maze maze = parse(kValid);
    EXPECT_EQ(maze.getName(), "Tiny");
    EXPECT_EQ(maze.getStartingCoins(), 3);
    EXPECT_EQ(maze.getTimeLimitSeconds(), 45);
    EXPECT_EQ(maze.getRows(), 4);
    EXPECT_EQ(maze.getCols(), 7);
    EXPECT_EQ(maze.getStart(), (Position{1, 1}));
    EXPECT_EQ(maze.getExit(), (Position{1, 5}));
    EXPECT_TRUE(maze.isWall({0, 0}));
    ASSERT_NE(maze.doorAt({1, 3}), nullptr);
    EXPECT_EQ(maze.doorAt({1, 3})->getCoinChange(), -2);
    ASSERT_NE(maze.doorAt({2, 3}), nullptr);
    EXPECT_EQ(maze.doorAt({2, 3})->getCoinChange(), 1);
}

TEST(MazeParserTest, AcceptsWindowsLineEndings) {
    std::string text = kValid;
    for (std::size_t at = text.find('\n'); at != std::string::npos; at = text.find('\n', at + 2)) {
        text.insert(at, "\r");
    }
    EXPECT_EQ(parse(text).getCols(), 7);
}

class MazeParserErrorTest : public ::testing::TestWithParam<std::pair<std::string, std::string>> {};

TEST_P(MazeParserErrorTest, RejectsMalformedMaze) {
    const auto& [from, to] = GetParam();
    EXPECT_THROW(parse(variant(from, to)), MazeError);
}

INSTANTIATE_TEST_SUITE_P(
    BrokenFiles, MazeParserErrorTest,
    ::testing::Values(std::make_pair("name: Tiny\n", ""),                    // no name
                      std::make_pair("coins: 3", "coins: three"),            // not a number
                      std::make_pair("time: 45\n", ""),                      // no time
                      std::make_pair("door a paid 2", "door a free 2"),      // unknown kind
                      std::make_pair("door b reward 1", "door a reward 1"),  // duplicate door
                      std::make_pair("#S.a.E#", "#S.z.E#"),                  // undeclared door
                      std::make_pair("#..b..#", "#..b.#"),                   // ragged row
                      std::make_pair("#S.a.E#", "#S.a.##"),                  // no exit
                      std::make_pair("time: 45", "speed: 45"),               // unknown setting
                      std::make_pair("grid:\n", "")));                       // no grid
