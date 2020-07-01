#pragma once

#include "ActivityTracker.h"

#include "editor/Object.h"

#include <QDebug>

#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QPen>

#include <memory>

struct Position
{
    int m_x;
    int m_y;
};

class Actor : public QGraphicsRectItem,
              public std::enable_shared_from_this<Actor>
{
public:
    Actor(
        const Position& position,
        const QRectF& coords,
        const std::optional<Object>& object,
        const std::shared_ptr<QGraphicsPixmapItem>& icon,
        const std::shared_ptr<QGraphicsTextItem>& alias,
        const std::shared_ptr<ActivityTracker>& tracker);

    void toggleActivated();

    QRectF coords();

    std::optional<Object> object();
    void setObject(const std::optional<Object>& object);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*);

private:
    Position m_position;
    QRectF m_coords;
    std::optional<Object> m_object;
    std::shared_ptr<QGraphicsPixmapItem> m_icon;
    std::shared_ptr<QGraphicsTextItem> m_alias;
    std::shared_ptr<ActivityTracker> m_tracker;
    bool m_isActive;
};

class ActorProxy : public QGraphicsProxyWidget
{
public:
    ActorProxy(const std::shared_ptr<Actor> actor);

    std::shared_ptr<Actor> value();

private:
    std::shared_ptr<Actor> m_actor;
};
