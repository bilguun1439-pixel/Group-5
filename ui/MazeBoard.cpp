#include "MazeBoard.h"

#include <QPainter>
#include <algorithm>

#include "GameSession.h"

namespace {
const QColor kWallColor(52, 58, 64);
const QColor kFloorColor(248, 249, 250);
const QColor kGridColor(222, 226, 230);
const QColor kStartColor(208, 235, 255);
const QColor kExitColor(81, 207, 102);
const QColor kPaidDoorColor(201, 120, 54);
const QColor kRewardDoorColor(245, 189, 31);
const QColor kOpenDoorColor(233, 236, 239);
const QColor kPlayerColor(34, 139, 230);
} // namespace

MazeBoard::MazeBoard(QWidget* parent) : QWidget(parent) {
    setMinimumSize(320, 240);
    setFocusPolicy(Qt::NoFocus); // keys go to GameView
}

void MazeBoard::setSession(const GameSession* newSession) {
    session = newSession;
    update();
}

void MazeBoard::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), palette().window());
    if (session == nullptr) {
        return;
    }

    const Maze& maze = session->getMaze();
    const int cell = std::max(1, std::min(width() / maze.getCols(), height() / maze.getRows()));
    const int offsetX = (width() - cell * maze.getCols()) / 2;
    const int offsetY = (height() - cell * maze.getRows()) / 2;

    QFont font = painter.font();
    font.setPixelSize(std::max(8, cell / 3));
    font.setBold(true);
    painter.setFont(font);

    for (int row = 0; row < maze.getRows(); ++row) {
        for (int col = 0; col < maze.getCols(); ++col) {
            const Position position{row, col};
            const QRect box(offsetX + col * cell, offsetY + row * cell, cell, cell);

            QColor fill = kFloorColor;
            if (maze.isWall(position)) {
                fill = kWallColor;
            } else if (maze.isExit(position)) {
                fill = kExitColor;
            } else if (position == maze.getStart()) {
                fill = kStartColor;
            }

            const Door* door = maze.doorAt(position);
            if (door != nullptr) {
                const bool reward = door->getCoinChange() > 0;
                fill = door->isOpen() ? kOpenDoorColor
                                      : (reward ? kRewardDoorColor : kPaidDoorColor);
            }

            painter.fillRect(box, fill);
            painter.setPen(kGridColor);
            painter.drawRect(box);

            if (door != nullptr && !door->isOpen()) {
                const int change = door->getCoinChange();
                painter.setPen(Qt::black);
                painter.drawText(box, Qt::AlignCenter,
                                 (change > 0 ? QString("+%1") : QString("%1")).arg(change));
            } else if (maze.isExit(position)) {
                painter.setPen(Qt::black);
                painter.drawText(box, Qt::AlignCenter, "EXIT");
            }
        }
    }

    const Position player = session->getPlayerPosition();
    const int margin = std::max(2, cell / 6);
    const QRect playerBox(offsetX + player.col * cell + margin, offsetY + player.row * cell + margin,
                          cell - 2 * margin, cell - 2 * margin);
    painter.setPen(Qt::NoPen);
    painter.setBrush(kPlayerColor);
    painter.drawEllipse(playerBox);
}
