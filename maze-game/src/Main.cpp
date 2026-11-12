// Main.cpp - application entry point.
//
// Responsibility: start the Qt application, show the Game window, and enter
// Qt's event loop. Keep this file tiny; all game logic belongs in the other
// classes.

#include <QApplication>

#include "Game.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Game game;
    game.show();

    return app.exec(); // runs until the window is closed
}
