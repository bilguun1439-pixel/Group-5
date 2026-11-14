#ifndef COINPATH_GAMETIMER_H
#define COINPATH_GAMETIMER_H

#include <chrono>

#include "IClock.h"

// GameTimer - the countdown for one game (FR5).
//
// Remaining time is worked out from the injected clock rather than counted
// down by ticks, so it stays correct even if the UI's refresh timer is late.
class GameTimer {
public:
    // Throws std::invalid_argument unless timeLimitSeconds is positive.
    // The clock must outlive the timer.
    GameTimer(int timeLimitSeconds, const IClock& clock);

    void start(); // no effect if already started
    void stop();  // freezes the remaining time; no effect unless running

    bool isRunning() const;
    int getTimeLimitSeconds() const;

    // Whole seconds left, rounded up, so it reads 0 only once time is up.
    int getRemainingSeconds() const;
    bool hasExpired() const;

private:
    std::chrono::milliseconds elapsed() const;

    const IClock& clock;
    int timeLimitSeconds;
    std::chrono::milliseconds startedAt{0};
    std::chrono::milliseconds stoppedAt{0};
    bool started = false;
    bool running = false;
};

#endif // COINPATH_GAMETIMER_H
