#include <QApplication>
#include "player.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    std::setlocale(LC_NUMERIC, "C");
    MainWindow   w;
    w.show();
    return app.exec();
}
