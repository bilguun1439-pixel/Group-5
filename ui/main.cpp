// Entry point: builds the real implementations of the external boundaries
// (maze files and the system clock) and injects them into the UI.

#include <QApplication>
#include <QCoreApplication>
#include <filesystem>

#include "FileMazeSource.h"
#include "IClock.h"
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // The build copies assets/mazes next to the executable.
    const std::filesystem::path mazeDirectory =
        std::filesystem::path(QCoreApplication::applicationDirPath().toStdU16String()) / "mazes";

    FileMazeSource mazeSource(mazeDirectory);
    SystemClock clock;

    MainWindow window(mazeSource, clock);
    window.show();
    return app.exec();
}
