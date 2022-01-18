#include "player.hpp"
#include <mpv/client.h>

#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidget>
#include <QDebug>

#include <cstdint>
#include <stdexcept>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , bar(new QProgressBar)
    , container_(new QWidget(this))
    , player_(new MpvPlayer(container_, this)) {
    auto cent = new QWidget(this);
    layout    = new QHBoxLayout(cent);
    setCentralWidget(cent);

    layout->addWidget(container_);
    layout->addWidget(bar);

    connect(player_, &MpvPlayer::positioinChanged, [this](double pos) {
        bar->setMaximum(player_->duration());
        bar->setValue(pos);
    });

    auto* menu   = menuBar()->addMenu("文件");
    auto* action = menu->addAction("打开");

    connect(action, &QAction::triggered, [this]() {
        QString filename = QFileDialog::getOpenFileName();
        player_->addMedia(filename);
        this->show();
    });
}

MainWindow::~MainWindow() { }
