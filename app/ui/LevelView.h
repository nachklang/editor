#pragma once

#include "Actor.h"

#include "ActorDisplayController.h"

#include "LevelController.h"

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
        const std::shared_ptr<LevelController>& levelController);

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
