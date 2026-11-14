#include "GameView.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "GameSession.h"
#include "MazeBoard.h"

namespace {
constexpr int kTickIntervalMs = 200;
constexpr int kLowTimeSeconds = 10;
} // namespace

GameView::GameView(QWidget* parent)
    : QWidget(parent),
      board(new MazeBoard(this)),
      titleLabel(new QLabel(this)),
      coinsLabel(new QLabel(this)),
      timeLabel(new QLabel(this)),
      messageLabel(new QLabel(this)),
      menuButton(new QPushButton(tr("Back to maze list"), this)) {
    setFocusPolicy(Qt::StrongFocus);
    menuButton->setFocusPolicy(Qt::NoFocus); // keep arrow keys on the board

    QFont hudFont = font();
    hudFont.setPointSize(hudFont.pointSize() + 3);
    hudFont.setBold(true);
    for (QLabel* label : {titleLabel, coinsLabel, timeLabel}) {
        label->setFont(hudFont);
    }
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setMinimumHeight(24);

    auto* hud = new QHBoxLayout;
    hud->addWidget(titleLabel);
    hud->addStretch();
    hud->addWidget(coinsLabel);
    hud->addSpacing(24);
    hud->addWidget(timeLabel);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(hud);
    layout->addWidget(board, 1);
    layout->addWidget(messageLabel);
    layout->addWidget(menuButton, 0, Qt::AlignRight);

    tickTimer.setInterval(kTickIntervalMs);
    connect(&tickTimer, &QTimer::timeout, this, [this] {
        if (session != nullptr) {
            session->tick();
        }
    });
    connect(menuButton, &QPushButton::clicked, this, &GameView::backToMenuRequested);
}

GameView::~GameView() {
    detach();
}

void GameView::attach(GameSession& newSession) {
    detach();
    session = &newSession;
    session->addObserver(*this);
    board->setSession(session);

    titleLabel->setText(QString::fromStdString(session->getMaze().getName()));
    messageLabel->setText(tr("Use the arrow keys to reach the exit."));
    showCoins(session->getCoins());
    showTime(session->getRemainingSeconds());

    session->start();
    tickTimer.start();
    setFocus();
}

void GameView::detach() {
    tickTimer.stop();
    if (session != nullptr) {
        session->removeObserver(*this);
        session = nullptr;
    }
    board->setSession(nullptr);
}

void GameView::keyPressEvent(QKeyEvent* event) {
    if (session == nullptr) {
        QWidget::keyPressEvent(event);
        return;
    }
    switch (event->key()) {
    case Qt::Key_Up:    session->move(Direction::Up); break;
    case Qt::Key_Down:  session->move(Direction::Down); break;
    case Qt::Key_Left:  session->move(Direction::Left); break;
    case Qt::Key_Right: session->move(Direction::Right); break;
    default:            QWidget::keyPressEvent(event); break;
    }
}

void GameView::onPlayerMoved(Position /*position*/) {
    messageLabel->clear();
    board->update();
}

void GameView::onCoinsChanged(int coins) {
    showCoins(coins);
    board->update(); // the door that was paid for is now drawn open
}

void GameView::onTimeChanged(int remainingSeconds) {
    showTime(remainingSeconds);
}

void GameView::onStatusChanged(GameStatus status) {
    switch (status) {
    case GameStatus::Won:
        tickTimer.stop();
        messageLabel->setText(tr("You reached the exit with %1 s to spare!")
                                  .arg(session->getRemainingSeconds()));
        break;
    case GameStatus::TimedOut:
        tickTimer.stop();
        messageLabel->setText(tr("Time's up! Try again from the maze list."));
        break;
    case GameStatus::NotStarted:
    case GameStatus::Running:
        break;
    }
}

void GameView::onMoveRejected(MoveResult reason) {
    switch (reason) {
    case MoveResult::Blocked:
        messageLabel->setText(tr("A wall is in the way."));
        break;
    case MoveResult::NotEnoughCoins:
        messageLabel->setText(tr("Not enough coins to open that door."));
        break;
    case MoveResult::GameNotRunning:
    case MoveResult::Moved:
        break;
    }
}

void GameView::showCoins(int coins) {
    coinsLabel->setText(tr("Coins: %1").arg(coins));
}

void GameView::showTime(int remainingSeconds) {
    timeLabel->setText(tr("Time: %1:%2")
                           .arg(remainingSeconds / 60)
                           .arg(remainingSeconds % 60, 2, 10, QLatin1Char('0')));
    timeLabel->setStyleSheet(remainingSeconds <= kLowTimeSeconds ? "color: #e03131;" : "");
}
