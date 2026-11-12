#ifndef GAME_H
#define GAME_H

#include <QWidget>

#include "GameTimer.h"
#include "Maze.h"
#include "MazeSelector.h"
#include "Player.h"

class QKeyEvent;
class QPaintEvent;

// Game - the game window and central controller.
//
// Responsibility: own the current Maze, Player, GameTimer and MazeSelector,
// connect them together, draw the game state, and receive keyboard input.
// Game is the only class (besides Main) that uses Qt; the other classes are
// plain C++ so they can be written and tested without the GUI.
//
// Future work (not implemented yet):
//  - arrow-key movement in keyPressEvent (FR1)
//  - drawing walls, doors and the exit in paintEvent (FR2)
//  - showing coins and remaining time (FR3, FR5)
//  - a QTimer that ticks GameTimer once per second (FR5)
//  - showing the solution path when time runs out (FR6)
class Game : public QWidget {
    Q_OBJECT

public:
    explicit Game(QWidget* parent = nullptr);

    const Maze& getMaze() const;
    const Player& getPlayer() const;
    const GameTimer& getTimer() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    static constexpr int kWindowWidth = 800;
    static constexpr int kWindowHeight = 600;
    static constexpr int kDefaultTimeLimitSeconds = 120;

    // Declaration order matters: members are constructed in this order.
    MazeSelector mazeSelector;
    Maze maze;
    Player player;
    GameTimer timer;
};

#endif // GAME_H
