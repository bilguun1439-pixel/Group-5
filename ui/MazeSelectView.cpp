#include "MazeSelectView.h"

#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "IMazeSource.h"

MazeSelectView::MazeSelectView(const IMazeSource& source, QWidget* parent)
    : QWidget(parent),
      source(source),
      list(new QListWidget(this)),
      playButton(new QPushButton(tr("Play"), this)) {
    auto* title = new QLabel(tr("Coin Path Dash"), this);
    QFont titleFont = title->font();
    titleFont.setPointSize(titleFont.pointSize() + 12);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);

    auto* prompt = new QLabel(tr("Choose a maze:"), this);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addSpacing(12);
    layout->addWidget(prompt);
    layout->addWidget(list, 1);
    layout->addWidget(playButton, 0, Qt::AlignRight);

    connect(playButton, &QPushButton::clicked, this, &MazeSelectView::chooseSelected);
    connect(list, &QListWidget::itemActivated, this, [this] { chooseSelected(); });

    refresh();
}

void MazeSelectView::refresh() {
    list->clear();
    for (const MazeInfo& maze : source.listMazes()) {
        auto* item = new QListWidgetItem(QString::fromStdString(maze.displayName), list);
        item->setData(Qt::UserRole, QString::fromStdString(maze.id));
    }

    const bool any = list->count() > 0;
    playButton->setEnabled(any);
    if (any) {
        list->setCurrentRow(0);
    } else {
        auto* empty = new QListWidgetItem(tr("No maze files were found."), list);
        empty->setFlags(Qt::NoItemFlags);
    }
}

void MazeSelectView::chooseSelected() {
    const QListWidgetItem* item = list->currentItem();
    if (item == nullptr || !(item->flags() & Qt::ItemIsEnabled)) {
        return;
    }
    emit mazeChosen(item->data(Qt::UserRole).toString());
}
