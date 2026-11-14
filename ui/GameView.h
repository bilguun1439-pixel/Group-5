#ifndef COINPATH_UI_GAMEVIEW_H
#define COINPATH_UI_GAMEVIEW_H

#include <QTimer>
#include <QWidget>

#include "IGameObserver.h"

class QLabel;
class QPushButton;
class GameSession;
class MazeBoard;

// GameView - the play screen: the board plus a HUD with coins, time and
// messages.
//
// It turns arrow keys into GameSession::move() calls and redraws when the
// session notifies it (it is a concrete Observer). It holds no game rules.
class GameView : public QWidget, public IGameObserver {
    Q_OBJECT

public:
    explicit GameView(QWidget* parent = nullptr);
    ~GameView() override;

    // Starts showing and driving this session. Not owned; call detach()
    // before the session is destroyed.
    void attach(GameSession& session);
    void detach();

signals:
    void backToMenuRequested();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    void onPlayerMoved(Position position) override;
    void onCoinsChanged(int coins) override;
    void onTimeChanged(int remainingSeconds) override;
    void onStatusChanged(GameStatus status) override;
    void onMoveRejected(MoveResult reason) override;

    void showCoins(int coins);
    void showTime(int remainingSeconds);

    GameSession* session = nullptr;
    MazeBoard* board;
    QLabel* titleLabel;
    QLabel* coinsLabel;
    QLabel* timeLabel;
    QLabel* messageLabel;
    QPushButton* menuButton;
    QTimer tickTimer;
};

#endif // COINPATH_UI_GAMEVIEW_H
