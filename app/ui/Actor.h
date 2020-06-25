#pragma once

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPen>

#include <QGraphicsRectItem>
#include "ActivityTracker.h"
#include "Rected.h"

#include <memory>

class Actor : public QGraphicsProxyWidget
{
public:
    Actor();

    Actor(int i, int j, QRectF coords, const std::shared_ptr<ActivityTracker>& statusHandler)
    {
        m_tracker = statusHandler;
        auto rect = new Rected{i, j, coords, statusHandler};
        rect->setAcceptHoverEvents(true);

        this->setGraphicsItem(rect);
    }
private:
    std::shared_ptr<ActivityTracker> m_tracker;
};
