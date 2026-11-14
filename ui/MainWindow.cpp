#include "MainWindow.h"

#include <QMessageBox>
#include <QStackedWidget>

#include "GameSession.h"
#include "GameView.h"
#include "IMazeSource.h"
#include "MazeError.h"
#include "MazeSelectView.h"

MainWindow::MainWindow(const IMazeSource& mazeSource, const IClock& clock, QWidget* parent)
    : QMainWindow(parent),
      mazeSource(mazeSource),
      clock(clock),
      pages(new QStackedWidget(this)),
      selectView(new MazeSelectView(mazeSource, pages)),
      gameView(new GameView(pages)) {
    setWindowTitle(tr("Coin Path Dash"));
    resize(820, 640);

    pages->addWidget(selectView);
    pages->addWidget(gameView);
    setCentralWidget(pages);

    connect(selectView, &MazeSelectView::mazeChosen, this, &MainWindow::startGame);
    connect(gameView, &GameView::backToMenuRequested, this, &MainWindow::showMazeList);
}

// GameView must stop observing the session before the session is destroyed.
MainWindow::~MainWindow() {
    gameView->detach();
}

void MainWindow::startGame(const QString& mazeId) {
    std::unique_ptr<GameSession> newSession;
    try {
        newSession = std::make_unique<GameSession>(mazeSource.loadMaze(mazeId.toStdString()), clock);
    } catch (const MazeError& error) {
        QMessageBox::warning(this, tr("Could not load maze"), QString::fromStdString(error.what()));
        return;
    }

    gameView->attach(*newSession); // detaches from the previous session first
    session = std::move(newSession);
    pages->setCurrentWidget(gameView);
    gameView->setFocus();
}

void MainWindow::showMazeList() {
    gameView->detach();
    session.reset();
    selectView->refresh();
    pages->setCurrentWidget(selectView);
}
