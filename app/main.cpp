#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto mainWindow = editor::MainWindow{};
    mainWindow.show();
    return app.exec();
}
