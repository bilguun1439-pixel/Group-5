#include <gtest/gtest.h>

#include <chrono>
#include <stdexcept>

#include "FakeClock.h"
#include "GameTimer.h"

using std::chrono::milliseconds;

TEST(GameTimerTest, FullTimeBeforeStart) {
    FakeClock clock;
    GameTimer timer(10, clock);
    clock.advance(milliseconds(5000));
    EXPECT_EQ(timer.getRemainingSeconds(), 10);
    EXPECT_FALSE(timer.isRunning());
}

TEST(GameTimerTest, CountsDownRoundingUp) {
    FakeClock clock;
    GameTimer timer(10, clock);
    timer.start();
    clock.advance(milliseconds(1));
    EXPECT_EQ(timer.getRemainingSeconds(), 10);
    clock.advance(milliseconds(1000));
    EXPECT_EQ(timer.getRemainingSeconds(), 9);
}

TEST(GameTimerTest, ExpiresAtTheLimit) {
    FakeClock clock;
    GameTimer timer(2, clock);
    timer.start();
    clock.advance(milliseconds(1999));
    EXPECT_FALSE(timer.hasExpired());
    clock.advance(milliseconds(1));
    EXPECT_TRUE(timer.hasExpired());
    clock.advance(milliseconds(5000));
    EXPECT_EQ(timer.getRemainingSeconds(), 0);
}

TEST(GameTimerTest, StopFreezesRemainingTime) {
    FakeClock clock;
    GameTimer timer(10, clock);
    timer.start();
    clock.advance(milliseconds(3000));
    timer.stop();
    clock.advance(milliseconds(60000));
    EXPECT_EQ(timer.getRemainingSeconds(), 7);
}

TEST(GameTimerTest, RejectsNonPositiveLimit) {
    FakeClock clock;
    EXPECT_THROW(GameTimer(0, clock), std::invalid_argument);
}
