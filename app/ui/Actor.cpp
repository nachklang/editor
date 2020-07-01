#include "Actor.h"

#include <QGraphicsScene>

Actor::Actor(
    const Position &position,
    const QRectF &coords,
    const std::optional<Object> &object,
    const std::shared_ptr<QGraphicsPixmapItem> &icon,
    const std::shared_ptr<QGraphicsTextItem> &alias,
    const std::shared_ptr<ActivityTracker> &tracker) :
  QGraphicsRectItem(coords),
  m_position(position),
  m_coords(coords),
  m_object(object),
  m_icon(icon),
  m_alias(alias),
  m_tracker(tracker),
  m_isActive(false)
{
}

void Actor::toggleActivated()
{
    m_isActive = !m_isActive;
    static auto pen =
        QPen(Qt::green, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    static auto blackPen = QPen(QColor("Black"));
    setPen(m_isActive ? pen : blackPen);
}

QRectF Actor::coords()
{
    return m_coords;
}

std::optional<Object> Actor::object()
{
    return m_object;
}

void Actor::setObject(const std::optional<Object> &object)
{
    m_object = object;
}

void Actor::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    toggleActivated();
    m_tracker->activated(this->shared_from_this());
}

ActorProxy::ActorProxy(const std::shared_ptr<Actor> actor) : m_actor(actor)
{
    setGraphicsItem(m_actor.get());
}

std::shared_ptr<Actor> ActorProxy::value()
{
    return m_actor;
}

