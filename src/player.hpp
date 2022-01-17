#pragma once

#include <QMainWindow>

#include "qmpv.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QWidget*   container_;
    MpvPlayer* player_;
};
