#pragma once

#include "Actor.h"

#include "ActorDisplayController.h"

#include "LevelController.h"

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QPushButton>
#include <QStyleOptionGraphicsItem>
#include <QTextEdit>
#include "editor/Properties.h"

#include <QWidget>

#include <QGraphicsGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QGraphicsRectItem>

#include <QLabel>
#include <QVBoxLayout>
#include <memory>

namespace editor
{

namespace
{

}

class LevelView : public QWidget
{
    Q_OBJECT
public:
    LevelView(
        const std::shared_ptr<ActivityTracker>& tracker,
        const std::shared_ptr<QGraphicsScene>& scene,
        const std::shared_ptr<ActorDisplayController>& displayController,
        const std::shared_ptr<LevelController>& levelController) :
      m_tracker(tracker),
      m_scene(scene),
      m_displayController(displayController),
      m_levelController(levelController)
    {
        auto layout = new QVBoxLayout;
        auto label = new QLabel("LEVEL VIEW WIDGET");
        layout->addWidget(label);

        auto view = new QGraphicsView;

        auto createSize = []() {
            return QSize{actor::BORDER * level::FIELD_COLUMNS + 50,
                         actor::BORDER * level::FIELD_ROWS + 50};
        };

        auto viewSize = createSize();

        view->setFixedSize(viewSize);

        auto grid = new QGraphicsGridLayout;
        grid->setColumnFixedWidth(0, actor::BORDER);
        grid->setRowFixedHeight(0, actor::BORDER);

        auto mini = new QGraphicsWidget;
        mini->setMinimumHeight(actor::BORDER);
        mini->setMinimumWidth(actor::BORDER);

        mini->paint(new QPainter, new QStyleOptionGraphicsItem);
        grid->addItem(mini, 0, 1);

        QObject::connect(
            tracker.get(),
            &ActivityTracker::activated,
            this,
            &LevelView::onActorActivated);

        static auto ptr_map =
            std::map<std::shared_ptr<ActorProxy>, std::shared_ptr<Actor>>{};

        for (auto i = 0; i < 10; ++i)
        {
            static auto a = 0.0;
            static auto b = 0.0;
            for (auto j = 0; j < 15; ++j)
            {

                static auto c = 30.0;
                static auto d = 30.0;

                auto icon = std::make_shared<QGraphicsPixmapItem>();
                auto alias = std::make_shared<QGraphicsTextItem>();

                auto actor = std::make_shared<Actor>(
                    Position{i, j},
                    QRectF{a, b, c, d},
                    std::optional<Object>{std::nullopt},
                    icon,
                    alias,
                    tracker);

                auto actorProxy = std::make_shared<ActorProxy>(actor);
                ptr_map[actorProxy] = actor;

                grid->addItem(actorProxy.get(), i, j);

                qDebug() << "i: " << i << "J: " << j;

                a += 30;
            }
            a = 0;
            b += 30;
        }

        m_displayController->setActorsScene(ptr_map);
        m_levelController->setActorsOnScene(ptr_map);

        qDebug() << grid->itemAt(10);

        m_scene->setSceneRect(0, 0, 14 * 30, 14 * 30);

        auto mainWindget = new QGraphicsWidget;
        mainWindget->setLayout(grid);

        m_scene->addItem(mainWindget);

        view->setScene(m_scene.get());
        layout->addWidget(view);

        setLayout(layout);

        QObject::connect(
            m_levelController.get(),
            &LevelController::showLoadedActor,
            m_displayController.get(),
            &ActorDisplayController::showRepresentation);

        QObject::connect(
            m_levelController.get(),
            &LevelController::hideDestroyedActor,
            m_displayController.get(),
            &ActorDisplayController::hideRepresentation);
    }

signals:
    void sendActiveActor(
        bool hasActivatedIndex, const std::shared_ptr<Actor>& actor);

public slots:
    void onActorActivated(const std::shared_ptr<Actor>& index);

private:
    std::vector<std::shared_ptr<Actor>> m_activatedActor;
    std::shared_ptr<ActivityTracker> m_tracker;
    std::shared_ptr<QGraphicsScene> m_scene;
    std::shared_ptr<ActorDisplayController> m_displayController;
    std::shared_ptr<LevelController> m_levelController;
};

} // namespace editor
