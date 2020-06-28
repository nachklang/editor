#pragma once

#include <QObject>

#include <memory>

class Rected;
class Actor;

class ActivityTracker: public QObject
{
    Q_OBJECT
signals:
    void activated(Rected* index);
    void activated(std::shared_ptr<Actor>& actor);
};
