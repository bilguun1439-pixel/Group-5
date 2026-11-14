#ifndef COINPATH_UI_MAZEBOARD_H
#define COINPATH_UI_MAZEBOARD_H

#include <QWidget>

class GameSession;

// MazeBoard - draws the maze, doors and player of a GameSession.
// Pure presentation: it reads the session and never changes it.
class MazeBoard : public QWidget {
public:
    explicit MazeBoard(QWidget* parent = nullptr);

    // The session to draw, or nullptr for an empty board. Not owned.
    void setSession(const GameSession* session);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    const GameSession* session = nullptr;
};

#endif // COINPATH_UI_MAZEBOARD_H
