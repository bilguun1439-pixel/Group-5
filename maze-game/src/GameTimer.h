#ifndef GAMETIMER_H
#define GAMETIMER_H

// GameTimer - the countdown timer for a game session.
//
// Responsibility: track how many seconds the player has left. It stays plain
// C++: Game will own a QTimer that calls into GameTimer once per second.
//
// Future work (not implemented yet):
//  - counting down once per second (FR5)
//  - signalling expiry so the solution path can be shown (FR6)
class GameTimer {
public:
    explicit GameTimer(int timeLimitSeconds);

    int getTimeLimitSeconds() const;
    int getSecondsRemaining() const;

private:
    int timeLimitSeconds;
    int secondsRemaining;
};

#endif // GAMETIMER_H
