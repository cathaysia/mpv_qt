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

#include <cstdint>
#include <stdexcept>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), container_(new QWidget(this)), player_(new MpvPlayer(container_, this)) {
    setCentralWidget(container_);

    auto* menu   = menuBar()->addMenu("文件");
    auto* action = menu->addAction("打开");

    connect(action, &QAction::triggered, [this]() {
        QString filename = QFileDialog::getOpenFileName();
        player_->setUrl(filename);
        player_->play();
    });
}

MainWindow::~MainWindow() { }
