#pragma once

#include <QMainWindow>

namespace editor {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void loadFile(const QString &fileName);
};

}
