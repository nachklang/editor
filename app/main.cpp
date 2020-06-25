#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{


    QApplication app(argc, argv);
    //    QCoreApplication::setOrganizationName("QtProject");
    //    QCoreApplication::setApplicationName("Application Example");
    //    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    //    QCommandLineParser parser;
    //    parser.setApplicationDescription(QCoreApplication::applicationName());
    //    parser.addHelpOption();
    //    parser.addVersionOption();
    //    parser.addPositionalArgument("file", "The file to open.");
    //    parser.process(app);

    auto mainWindow = editor::MainWindow{};
    mainWindow.show();
    return app.exec();
}
