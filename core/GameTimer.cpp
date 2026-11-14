#include "GameTimer.h"

#include <stdexcept>

GameTimer::GameTimer(int timeLimitSeconds, const IClock& clock)
    : clock(clock), timeLimitSeconds(timeLimitSeconds) {
    if (timeLimitSeconds <= 0) {
        throw std::invalid_argument("The time limit must be positive");
    }
}

void GameTimer::start() {
    if (started) {
        return;
    }
    started = true;
    running = true;
    startedAt = clock.now();
}

void GameTimer::stop() {
    if (!running) {
        return;
    }
    running = false;
    stoppedAt = clock.now();
}

bool GameTimer::isRunning() const {
    return running;
}

int GameTimer::getTimeLimitSeconds() const {
    return timeLimitSeconds;
}

int GameTimer::getRemainingSeconds() const {
    const long long remainingMs =
        static_cast<long long>(timeLimitSeconds) * 1000 - elapsed().count();
    if (remainingMs <= 0) {
        return 0;
    }
    return static_cast<int>((remainingMs + 999) / 1000);
}

bool GameTimer::hasExpired() const {
    return getRemainingSeconds() == 0;
}

std::chrono::milliseconds GameTimer::elapsed() const {
    if (!started) {
        return std::chrono::milliseconds{0};
    }
    return (running ? clock.now() : stoppedAt) - startedAt;
}
