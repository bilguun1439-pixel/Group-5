#ifndef COINPATH_UI_MAZESELECTVIEW_H
#define COINPATH_UI_MAZESELECTVIEW_H

#include <QString>
#include <QWidget>

class IMazeSource;
class QListWidget;
class QPushButton;

// MazeSelectView - the start screen, listing the available mazes (FR8).
// Emits mazeChosen with the id of the maze the player picks.
class MazeSelectView : public QWidget {
    Q_OBJECT

public:
    // The source must outlive the view.
    explicit MazeSelectView(const IMazeSource& source, QWidget* parent = nullptr);

    // Re-reads the list of mazes from the source.
    void refresh();

signals:
    void mazeChosen(const QString& mazeId);

private:
    void chooseSelected();

    const IMazeSource& source;
    QListWidget* list;
    QPushButton* playButton;
};

#endif // COINPATH_UI_MAZESELECTVIEW_H
