#ifndef COINPATH_GAMESESSION_H
#define COINPATH_GAMESESSION_H

#include <vector>

#include "GameTimer.h"
#include "GameTypes.h"
#include "IClock.h"
#include "IGameObserver.h"
#include "Maze.h"
#include "Player.h"

// GameSession - one play-through of one maze, and the only entry point the
// UI uses to change game state.
//
// It enforces the movement and coin rules (FR1-FR3), runs the timer (FR5),
// moves the game through its life cycle, and tells observers about every
// change (it is the Subject of the Observer pattern).
class GameSession {
public:
    // The clock must outlive the session.
    GameSession(Maze maze, const IClock& clock);

    GameSession(const GameSession&) = delete;
    GameSession& operator=(const GameSession&) = delete;

    // Observers are not owned. An observer must call removeObserver() before
    // it is destroyed. Adding the same observer twice has no effect.
    void addObserver(IGameObserver& observer);
    void removeObserver(IGameObserver& observer);

    // NotStarted -> Running, and starts the timer. Returns false otherwise.
    bool start();

    // Tries to move the player one cell. A rejected move changes nothing.
    MoveResult move(Direction direction);

    // Called regularly by the UI: reports the time left and ends the game
    // when the time runs out.
    void tick();

    GameStatus getStatus() const;
    const Maze& getMaze() const;
    Position getPlayerPosition() const;
    int getCoins() const;
    int getRemainingSeconds() const;

private:
    // Ends the game if time is up. Returns true if the game is still running.
    bool checkTimeout();
    void setStatus(GameStatus newStatus);
    MoveResult reject(MoveResult reason);

    void notifyPlayerMoved();
    void notifyCoinsChanged();
    void notifyTimeChanged();
    void notifyStatusChanged();

    Maze maze;
    Player player;
    GameTimer timer;
    GameStatus status = GameStatus::NotStarted;
    int lastReportedSeconds;
    std::vector<IGameObserver*> observers; // not owned
};

#endif // COINPATH_GAMESESSION_H
