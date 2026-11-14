#ifndef COINPATH_UI_MAINWINDOW_H
#define COINPATH_UI_MAINWINDOW_H

#include <QMainWindow>
#include <memory>

class GameSession;
class GameView;
class IClock;
class IMazeSource;
class MazeSelectView;
class QStackedWidget;

// MainWindow - switches between the maze list and the play screen, and owns
// the GameSession being played.
//
// This is where the UI reaches the domain: it asks the injected IMazeSource
// for a maze, creates a GameSession with the injected IClock, and hands the
// session to GameView. Both dependencies are passed in from main().
class MainWindow : public QMainWindow {
public:
    // The source and clock must outlive the window.
    MainWindow(const IMazeSource& mazeSource, const IClock& clock, QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    void startGame(const QString& mazeId);
    void showMazeList();

    const IMazeSource& mazeSource;
    const IClock& clock;
    QStackedWidget* pages;
    MazeSelectView* selectView;
    GameView* gameView;
    std::unique_ptr<GameSession> session;
};

#endif // COINPATH_UI_MAINWINDOW_H
