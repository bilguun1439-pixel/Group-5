#include "GameSession.h"

#include <algorithm>
#include <utility>

GameSession::GameSession(Maze maze, const IClock& clock)
    : maze(std::move(maze)),
      player(this->maze.getStart(), this->maze.getStartingCoins()),
      timer(this->maze.getTimeLimitSeconds(), clock),
      lastReportedSeconds(timer.getRemainingSeconds()) {}

void GameSession::addObserver(IGameObserver& observer) {
    if (std::find(observers.begin(), observers.end(), &observer) == observers.end()) {
        observers.push_back(&observer);
    }
}

void GameSession::removeObserver(IGameObserver& observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), &observer),
                    observers.end());
}

bool GameSession::start() {
    if (status != GameStatus::NotStarted) {
        return false;
    }
    timer.start();
    setStatus(GameStatus::Running);
    return true;
}

MoveResult GameSession::move(Direction direction) {
    if (!checkTimeout()) {
        return reject(MoveResult::GameNotRunning);
    }

    const Position destination = step(player.getPosition(), direction);
    if (maze.isWall(destination)) {
        return reject(MoveResult::Blocked);
    }

    if (Door* door = maze.doorAt(destination)) {
        const int coinsBefore = player.getWallet().getBalance();
        if (!door->tryPass(player.getWallet())) {
            return reject(MoveResult::NotEnoughCoins);
        }
        if (player.getWallet().getBalance() != coinsBefore) {
            notifyCoinsChanged();
        }
    }

    player.moveTo(destination);
    notifyPlayerMoved();

    if (maze.isExit(destination)) {
        timer.stop();
        setStatus(GameStatus::Won);
    }
    return MoveResult::Moved;
}

void GameSession::tick() {
    if (!checkTimeout()) {
        return;
    }
    const int remaining = timer.getRemainingSeconds();
    if (remaining != lastReportedSeconds) {
        lastReportedSeconds = remaining;
        notifyTimeChanged();
    }
}

GameStatus GameSession::getStatus() const {
    return status;
}

const Maze& GameSession::getMaze() const {
    return maze;
}

Position GameSession::getPlayerPosition() const {
    return player.getPosition();
}

int GameSession::getCoins() const {
    return player.getWallet().getBalance();
}

int GameSession::getRemainingSeconds() const {
    return timer.getRemainingSeconds();
}

bool GameSession::checkTimeout() {
    if (status != GameStatus::Running) {
        return false;
    }
    if (!timer.hasExpired()) {
        return true;
    }
    timer.stop();
    lastReportedSeconds = 0;
    notifyTimeChanged();
    setStatus(GameStatus::TimedOut);
    return false;
}

void GameSession::setStatus(GameStatus newStatus) {
    status = newStatus;
    notifyStatusChanged();
}

MoveResult GameSession::reject(MoveResult reason) {
    for (IGameObserver* observer : observers) {
        observer->onMoveRejected(reason);
    }
    return reason;
}

void GameSession::notifyPlayerMoved() {
    for (IGameObserver* observer : observers) {
        observer->onPlayerMoved(player.getPosition());
    }
}

void GameSession::notifyCoinsChanged() {
    for (IGameObserver* observer : observers) {
        observer->onCoinsChanged(getCoins());
    }
}

void GameSession::notifyTimeChanged() {
    for (IGameObserver* observer : observers) {
        observer->onTimeChanged(lastReportedSeconds);
    }
}

void GameSession::notifyStatusChanged() {
    for (IGameObserver* observer : observers) {
        observer->onStatusChanged(status);
    }
}
