#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>

#include "FakeClock.h"
#include "GameSession.h"
#include "MazeBuilder.h"

using ::testing::_;
using ::testing::InSequence;
using ::testing::NiceMock;
using std::chrono::milliseconds;

namespace {

class MockObserver : public IGameObserver {
public:
    MOCK_METHOD(void, onPlayerMoved, (Position), (override));
    MOCK_METHOD(void, onCoinsChanged, (int), (override));
    MOCK_METHOD(void, onTimeChanged, (int), (override));
    MOCK_METHOD(void, onStatusChanged, (GameStatus), (override));
    MOCK_METHOD(void, onMoveRejected, (MoveResult), (override));
};

MATCHER_P2(IsAt, row, col, "") {
    return arg.row == row && arg.col == col;
}

// Row 0:  S . a . E      a = paid door (cost 3)
// Row 1:  # # b # #      b = reward door (+2), a dead end below the start row
Maze makeMaze(int startingCoins) {
    MazeBuilder builder("Test", 2, 5);
    builder.setStart({0, 0})
        .setExit({0, 4})
        .addPaidDoor({0, 2}, 3)
        .addWall({1, 0})
        .addWall({1, 1})
        .addRewardDoor({1, 2}, 2)
        .addWall({1, 3})
        .addWall({1, 4})
        .setStartingCoins(startingCoins)
        .setTimeLimitSeconds(30);
    return builder.build();
}

class GameSessionTest : public ::testing::Test {
protected:
    FakeClock clock;
};

} // namespace

TEST_F(GameSessionTest, MovesOnlyAfterStart) {
    GameSession session(makeMaze(5), clock);
    EXPECT_EQ(session.move(Direction::Right), MoveResult::GameNotRunning);
    EXPECT_TRUE(session.start());
    EXPECT_FALSE(session.start()); // NotStarted -> Running happens once
    EXPECT_EQ(session.move(Direction::Right), MoveResult::Moved);
    EXPECT_EQ(session.getPlayerPosition(), (Position{0, 1}));
}

TEST_F(GameSessionTest, WallsAndEdgesBlockWithoutChangingState) {
    GameSession session(makeMaze(5), clock);
    session.start();
    EXPECT_EQ(session.move(Direction::Up), MoveResult::Blocked);   // edge
    EXPECT_EQ(session.move(Direction::Down), MoveResult::Blocked); // wall
    EXPECT_EQ(session.getPlayerPosition(), (Position{0, 0}));
    EXPECT_EQ(session.getCoins(), 5);
}

TEST_F(GameSessionTest, PaidDoorChargesOnceAndReachingExitWins) {
    GameSession session(makeMaze(3), clock);
    session.start();
    session.move(Direction::Right);
    EXPECT_EQ(session.move(Direction::Right), MoveResult::Moved); // through the door
    EXPECT_EQ(session.getCoins(), 0);
    session.move(Direction::Left);
    EXPECT_EQ(session.move(Direction::Right), MoveResult::Moved); // already open: free
    EXPECT_EQ(session.getCoins(), 0);
    session.move(Direction::Right);
    session.move(Direction::Right);
    EXPECT_EQ(session.getStatus(), GameStatus::Won);
    EXPECT_EQ(session.move(Direction::Left), MoveResult::GameNotRunning);
}

TEST_F(GameSessionTest, UnaffordableDoorIsRejected) {
    GameSession session(makeMaze(1), clock);
    session.start();
    session.move(Direction::Right);
    EXPECT_EQ(session.move(Direction::Right), MoveResult::NotEnoughCoins);
    EXPECT_EQ(session.getPlayerPosition(), (Position{0, 1}));
    EXPECT_EQ(session.getCoins(), 1);
    EXPECT_FALSE(session.getMaze().doorAt({0, 2})->isOpen());
}

TEST_F(GameSessionTest, TimeoutEndsTheGame) {
    GameSession session(makeMaze(5), clock);
    session.start();
    clock.advance(milliseconds(30000));
    EXPECT_EQ(session.move(Direction::Right), MoveResult::GameNotRunning);
    EXPECT_EQ(session.getStatus(), GameStatus::TimedOut);
    EXPECT_EQ(session.getPlayerPosition(), (Position{0, 0}));
}

TEST_F(GameSessionTest, ObserversHearAboutAWinningRunInOrder) {
    GameSession session(makeMaze(3), clock);
    MockObserver observer;
    session.addObserver(observer);
    {
        InSequence order;
        EXPECT_CALL(observer, onStatusChanged(GameStatus::Running)).Times(1);
        EXPECT_CALL(observer, onPlayerMoved(IsAt(0, 1))).Times(1);
        EXPECT_CALL(observer, onCoinsChanged(0)).Times(1);
        EXPECT_CALL(observer, onPlayerMoved(IsAt(0, 2))).Times(1);
        EXPECT_CALL(observer, onPlayerMoved(IsAt(0, 3))).Times(1);
        EXPECT_CALL(observer, onPlayerMoved(IsAt(0, 4))).Times(1);
        EXPECT_CALL(observer, onStatusChanged(GameStatus::Won)).Times(1);
    }
    EXPECT_CALL(observer, onMoveRejected(_)).Times(0);

    session.start();
    for (int i = 0; i < 4; ++i) {
        session.move(Direction::Right);
    }
}

TEST_F(GameSessionTest, ObserversHearRejectionsAndTimeout) {
    GameSession session(makeMaze(0), clock);
    NiceMock<MockObserver> observer;
    session.addObserver(observer);
    session.start();

    EXPECT_CALL(observer, onMoveRejected(MoveResult::Blocked)).Times(1);
    session.move(Direction::Up);

    EXPECT_CALL(observer, onTimeChanged(29)).Times(1);
    clock.advance(milliseconds(1500));
    session.tick();
    session.tick(); // same second: no repeat notification

    EXPECT_CALL(observer, onTimeChanged(0)).Times(1);
    EXPECT_CALL(observer, onStatusChanged(GameStatus::TimedOut)).Times(1);
    clock.advance(milliseconds(30000));
    session.tick();
}

TEST_F(GameSessionTest, RemovedObserverIsNotNotified) {
    GameSession session(makeMaze(5), clock);
    MockObserver observer;
    session.addObserver(observer);
    session.removeObserver(observer);
    EXPECT_CALL(observer, onStatusChanged(_)).Times(0);
    session.start();
}
