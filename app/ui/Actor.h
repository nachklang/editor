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


class Actor : public QGraphicsProxyWidget
{
public:
    Actor();

    Actor(int i, int j, QRectF coords, const std::shared_ptr<ActivityTracker>& statusHandler, QGraphicsScene* scene)
    {
        m_tracker = statusHandler;
        auto rect = new Rected{i, j, coords, statusHandler, scene};
        rect->setAcceptHoverEvents(true);

        this->setGraphicsItem(rect);
    }
private:
    Position m_position;
    std::optional<Object> m_object;
    std::shared_ptr<ActivityTracker> m_tracker;
};
