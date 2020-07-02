#include "MainWindow.h"

#include "ActorDisplayController.h"
#include "ActorEditor.h"

#include "LevelController.h"
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

#include <memory>

namespace editor {

namespace {

constexpr auto OBJECTS_FILE_NAME = "objects/objects.json";

Objects readObjectsFromConfig()
{
    auto reader = ConfigReader{};
    auto object = reader.createJsonObjectFromFile(OBJECTS_FILE_NAME);
    auto types = reader.readObjectsFromJson(object);

    return types;
}

QWidget* CreateMainWindget(std::shared_ptr<LevelController>& levelController, const Objects& types)
{
    auto scene = std::make_shared<QGraphicsScene>();
    auto displayController = std::make_shared<ActorDisplayController>(scene);
    auto actorEditor = new ActorEditor(types, displayController);

    auto levelViewLayout = new QHBoxLayout;

    auto tracker =
        std::shared_ptr<ActivityTracker>(new ActivityTracker);

    auto levelView = new LevelView(tracker, scene, displayController, levelController);

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
    auto types = readObjectsFromConfig();
    auto levelController = std::make_shared<LevelController>(types);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new level"));
    connect(newAct, &QAction::triggered, levelController.get(), &LevelController::createNewLevel);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing level"));
    connect(openAct, &QAction::triggered, levelController.get(), &LevelController::openExistedLevel);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAction = new QAction(openIcon, tr("&Save..."), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save an existing level"));
    connect(saveAction, &QAction::triggered, levelController.get(), &LevelController::saveLevel);

    fileMenu->addAction(saveAction);
    fileToolBar->addAction(saveAction);


    auto mainWidget = CreateMainWindget(levelController, types);
    setCentralWidget(mainWidget);

    setFixedSize(700,410);
}

}
