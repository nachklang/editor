#pragma once

#include "editor/Object.h"

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPen>

#include <QGraphicsRectItem>
#include "ActivityTracker.h"
#include "Rected.h"

#include <memory>

struct Position
{
    int m_x;
    int m_y;
};

class Actor : public QGraphicsProxyWidget, std::enable_shared_from_this<Actor>
{
public:
    Actor();

    Actor(
        int i,
        int j,
        QRectF coords,
        const std::shared_ptr<ActivityTracker>& tracker,
        QGraphicsScene* scene)
    {
        m_tracker = tracker;
        auto rect = new Rected{i, j, coords, tracker, scene};
        //        rect->setAcceptHoverEvents(true);

        this->setGraphicsItem(rect);
    }

    Actor(
        Position position,
        QRect coords,
        std::optional<Object> object,
        std::shared_ptr<QGraphicsPixmapItem> icon,
        std::shared_ptr<QGraphicsTextItem> alias,
        std::shared_ptr<ActorCell2> cell,
        std::shared_ptr<ActivityTracker> tracker) :
      m_position(std::move(position)),
      m_coords(coords),
      m_object((object)),
      m_icon(icon),
      m_alias(alias),
      m_cell(cell),
      m_tracker(tracker)
    {
        this->setGraphicsItem(m_cell.get());

        QObject::connect(m_cell, &ActorCell2::cellActivated, this, [this]() {
            emit m_tracker->activated2(std::shared_ptr<Actor>(this));
        });
    }

    void mousePressEvent(QGraphicsSceneMouseEvent*)
    {
        qDebug() << "KIKIKIKIKI" << m_position.m_x << m_position.m_y;

        //  m_tracker->activated(this);
    }

private:
    Position m_position;
    QRectF m_coords;
    std::optional<Object> m_object;
    std::shared_ptr<QGraphicsPixmapItem> m_icon;
    std::shared_ptr<QGraphicsTextItem> m_alias;
    std::shared_ptr<ActorCell2> m_cell;
    std::shared_ptr<ActivityTracker> m_tracker;
};

class ActorNew
{
};
