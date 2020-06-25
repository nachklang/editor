#pragma once

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPen>

#include <QGraphicsRectItem>
#include "ActivityTracker.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTextDocument>

#include <memory>

class Rected :  public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Rected(int i, int j, QRectF coords, const std::shared_ptr<ActivityTracker>& statusHandler) :
      QGraphicsRectItem(coords),
      m_i(std::move(i)),
      m_j(std::move(j)),
      m_isActive(false),
      m_tracker(statusHandler)
    {
    }

    void toggleActivated()
    {
        m_isActive ? m_isActive = false : m_isActive = true;
        qDebug() << "Item i" << m_i << "Item j: " << m_j;
        static auto pen = QPen(Qt::green, 5, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
        static auto blackPen = QPen(QColor("Black"));
        setPen(m_isActive ? pen : blackPen);

        if (m_isActive)
        {
            m_textItem = new QGraphicsTextItem("A");
            // m_textItem->setFont(f);
            m_textItem->document()->setDocumentMargin(4);
            m_textItem->setDefaultTextColor(QColor(33, 33, 33));
            m_textItem->setPlainText(QObject::tr("User"));
            m_textItem->setPos(m_rect.x() - 15, m_rect.y() - 15);
        }
    }
signals:
    void activated(Rected* index);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *)
    {
       toggleActivated();

       m_tracker->activated(this);
    }


private:
    int m_i;
    int m_j;
    QRectF m_rect;
    bool m_isActive;
    std::shared_ptr<ActivityTracker> m_tracker;
    QGraphicsTextItem* m_textItem;
};
