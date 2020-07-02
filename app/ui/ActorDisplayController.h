#pragma once

#include "Actor.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

#include <map>
#include <memory>
#include <optional>

using IconData =
    std::pair<std::optional<QString>, std::shared_ptr<QGraphicsPixmapItem>>;
using AliasData =
    std::pair<std::optional<QString>, std::shared_ptr<QGraphicsTextItem>>;

struct ActorDisplayData
{
    IconData m_icon;
    AliasData m_alias;
};

using ActorsDisplayStorage = std::map<QString, ActorDisplayData>;

class ActorDisplayController: public QObject
{
    Q_OBJECT
public:
    ActorDisplayController(const std::shared_ptr<QGraphicsScene>& scene);

    void setActorsScene(const std::map<std::shared_ptr<ActorProxy>, std::shared_ptr<Actor>>& actorsOnScene);

    std::map<std::shared_ptr<ActorProxy>, std::shared_ptr<Actor>> actorsOnScene();

public slots:
    void showRepresentation(
        const QRectF coords,
        const std::optional<QString>& iconName,
        const std::optional<QString>& aliasName);

    void hideRepresentation(const QRectF& coords);

private:
    std::shared_ptr<QGraphicsScene> m_scene;
    ActorsDisplayStorage m_storage;
    std::map<std::shared_ptr<ActorProxy>, std::shared_ptr<Actor>> m_actorsOnScene;
};
