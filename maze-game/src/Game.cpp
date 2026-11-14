#include "Game.h"

#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>

// mazeSelector is built first, then the maze it loads, then the player placed
// at that maze's start position (see the member order in Game.h).
Game::Game(QWidget* parent)
    : QWidget(parent),
      mazeSelector(),
      maze(mazeSelector.loadMaze(0)),
      player(maze.getStartRow(), maze.getStartCol(), 0),
      timer(kDefaultTimeLimitSeconds) {
    setWindowTitle("Maze Game");
    setFixedSize(kWindowWidth, kWindowHeight);
    setFocusPolicy(Qt::StrongFocus); // so the window receives key presses
}

void Game::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    // TODO: draw the maze, player, coin count and timer.
    painter.setPen(Qt::black);
    painter.drawText(20, 30, "Maze Game - scaffolding is running.");
    painter.drawText(20, 50, QString("Loaded maze: %1 (%2x%3)")
                                 .arg(QString::fromStdString(maze.getName()))
                                 .arg(maze.getRows())
                                 .arg(maze.getCols()));
}

void Game::keyPressEvent(QKeyEvent* event) {
    // TODO: move the player with the arrow keys.
    QWidget::keyPressEvent(event);
}

// getter method for the maze
const Maze& Game::getMaze() const {
    return maze;
}
// getter method for the player
const Player& Game::getPlayer() const {
    return player;
}

const GameTimer& Game::getTimer() const {
    return timer;
}
