#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QProgressBar>

#include "qmpv.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QHBoxLayout*  layout;
    QProgressBar* bar;
    QWidget*      container_;
    MpvPlayer*    player_;
};
