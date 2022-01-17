#pragma once

#include <QMainWindow>

#include <mpv/client.h>
#include <qobjectdefs.h>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void mpv_event_changed();

private:
    void handle_mpv_event(mpv_event* event);
    void on_mpv_events();
    QWidget*    mpv_container;
    mpv_handle* mpv;

};
