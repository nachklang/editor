#pragma once

#include <QObject>

class Rected;

class ActivityTracker: public QObject
{
    Q_OBJECT
signals:
    void activated(Rected* index);
};
