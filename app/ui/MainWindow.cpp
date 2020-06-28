#include "MainWindow.h"

#include "ActorEditor.h"
#include "LevelView.h"
#include "editor/ConfigReader.h"
#include "editor/Property.h"

#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QMenuBar>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>

namespace editor {

namespace {

QWidget* CreateMainWindget()
{
    auto levelViewLayout = new QHBoxLayout;

    auto levelView = new LevelView;

    // READ MOCKS
    //auto actorEditor = new ActorEditor(std::optional<Objects>{});
    auto reader = ConfigReader{};
    qDebug() << "App path : " << qApp->applicationDirPath();
    auto object = reader.createJsonObjectFromFile("test_objects.json");
    auto types = reader.readObjectsFromJson(object);
    auto actorEditor = new ActorEditor(types);

    constexpr auto TEST_FILE_NAME = "readProperty.json";

    auto object2 = reader.createJsonObjectFromFile(TEST_FILE_NAME);

    auto properties = reader.readTypeProperties(object2);

    Q_UNUSED(properties)
    // END MOCKING


    levelViewLayout->addWidget(levelView);
    levelViewLayout->addWidget(new QSplitter);
    // ReadFromFile
    levelViewLayout->addWidget(actorEditor);

    QObject::connect(
        levelView,
        &LevelView::sendActivatedObject,
        actorEditor,
        &ActorEditor::receiveActivatedObject);

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

//        const QIcon exitIcon = QIcon::fromTheme("application-exit", QIcon(":/images/exit.png"));
//        QAction *openAct = new QAction(openIcon, tr("&Exit..."), this);
//        openAct->setShortcuts(QKeySequence::Open);
//        openAct->setStatusTip(tr("Exit from application"));
//        connect(openAct, &QAction::triggered, this, &MainWindow::close);


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
