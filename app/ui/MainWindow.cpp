#include "MainWindow.h"

#include "ActorEditor.h"
#include "LevelView.h"

#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QMenuBar>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace editor {

namespace {

QWidget* CreateMainWindget()
{
    auto levelViewLayout = new QHBoxLayout;
    levelViewLayout->addWidget(new LevelView);
    levelViewLayout->addWidget(new ActorEditor);

    auto mainLayout = new QVBoxLayout;

    mainLayout->addItem(levelViewLayout);

    mainLayout->setAlignment(Qt::AlignHCenter);

    auto widget = new QWidget;
    widget->setLayout(mainLayout);

    return widget;
}

} // namespace anonymous

MainWindow::MainWindow()
{


    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
//    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
//    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);
//    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
//    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));


    auto mainWidget = CreateMainWindget();
    setCentralWidget(mainWidget);

    setMinimumHeight(480);
    setMinimumWidth(640);
}

}
