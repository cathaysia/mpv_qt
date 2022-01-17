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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mpv(mpv_create()), mpv_container(new QWidget(this)) {
    if(!mpv) throw std::runtime_error("创建 mpv 实例失败");
    resize(800, 600);

    QMenu *menu   = menuBar()->addMenu("文件");
    auto   action = menu->addAction("打开文件");
    connect(action, &QAction::triggered, [this]() {
        if(!mpv) return;
        auto        filename = QFileDialog::getOpenFileName(this, "打开文件").toUtf8();
        const char *args[]   = { "loadfile", filename.data(), nullptr };
        mpv_command_async(mpv, 0, args);
    });

    setCentralWidget(mpv_container);
    mpv_container->setAttribute(Qt::WA_DontCreateNativeAncestors);
    mpv_container->setAttribute(Qt::WA_NativeWindow);

    int64_t wid = mpv_container->winId();
    mpv_set_option(mpv, "wid", MPV_FORMAT_INT64, &wid);

    connect(this, &MainWindow::mpv_event_changed, this, &MainWindow::on_mpv_events, Qt::QueuedConnection);

    mpv_set_wakeup_callback(
        mpv,
        [](void *data) {
            emit static_cast<MainWindow *>(data)->mpv_event_changed();
        },
        this);

    if(mpv_initialize(mpv) < 0) throw std::runtime_error("mpv 初始化失败");
}

MainWindow::~MainWindow() {
    if(mpv) mpv_terminate_destroy(mpv);
}

void MainWindow::handle_mpv_event(mpv_event *event) {
    if(event->event_id == MPV_EVENT_SHUTDOWN) {
        mpv_terminate_destroy(mpv);
        mpv = nullptr;
    }
}

void MainWindow::on_mpv_events() {
    mpv_event *event = mpv_wait_event(mpv, 0);
    if(event->event_id == MPV_EVENT_NONE) return;
    handle_mpv_event(event);
}
