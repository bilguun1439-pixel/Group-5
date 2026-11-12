#include "GameTimer.h"

GameTimer::GameTimer(int timeLimitSeconds)
    : timeLimitSeconds(timeLimitSeconds), secondsRemaining(timeLimitSeconds) {}

int GameTimer::getTimeLimitSeconds() const {
    return timeLimitSeconds;
}

int GameTimer::getSecondsRemaining() const {
    return secondsRemaining;
}
