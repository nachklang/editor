#pragma once

#include <QObject>

#include <memory>

class Actor;

class ActivityTracker: public QObject
{
    Q_OBJECT
signals:
    void activated(const std::shared_ptr<Actor>& actor);
};
