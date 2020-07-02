#include "MainWindow.h"

#include "ActorDisplayController.h"
#include "ActorEditor.h"

#include "LevelView.h"
#include "editor/ConfigReader.h"
#include "editor/Property.h"

#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QGraphicsScene>
#include <QIcon>
#include <QMenuBar>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>

namespace editor {

namespace {

constexpr auto OBJECTS_FILE_NAME = "objects/objects.json";

QWidget* CreateMainWindget()
{
    auto reader = ConfigReader{};
    qDebug() << "App path : " << qApp->applicationDirPath();
    auto object = reader.createJsonObjectFromFile(OBJECTS_FILE_NAME);
    auto types = reader.readObjectsFromJson(object);

    auto scene = std::make_shared<QGraphicsScene>();

    auto displayController = std::make_shared<ActorDisplayController>(scene);
    auto actorEditor = new ActorEditor(types, displayController);


    auto levelViewLayout = new QHBoxLayout;


    auto tracker =
        std::shared_ptr<ActivityTracker>(new ActivityTracker);

    auto levelView = new LevelView(tracker, scene, displayController);

    levelViewLayout->addWidget(levelView);
    levelViewLayout->addWidget(new QSplitter);
    levelViewLayout->addWidget(actorEditor);

    QObject::connect(
        tracker.get(),
        &ActivityTracker::activated,
        actorEditor,
        &ActorEditor::receiveActiveActor);

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

    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);
//    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
//    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));


    auto mainWidget = CreateMainWindget();
    setCentralWidget(mainWidget);

    setMinimumHeight(480);
    setMinimumWidth(800);
}

}
