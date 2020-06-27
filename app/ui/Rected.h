#pragma once

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPen>

#include <QGraphicsRectItem>
#include <QSvgRenderer>


#include <QGraphicsSvgItem>

#include "ActivityTracker.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTextDocument>

#include <QGraphicsScene>

#include <memory>

class Rected :  public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Rected(int i, int j, QRectF coords, const std::shared_ptr<ActivityTracker>& statusHandler, QGraphicsScene* scene) :
      QGraphicsRectItem(coords),
      m_i(std::move(i)),
      m_j(std::move(j)),
      m_isActive(false),
      m_tracker(statusHandler),
      m_scene(scene),
      m_coords(coords)
    {
    }

    void toggleActivated()
    {
        m_isActive ? m_isActive = false : m_isActive = true;
        qDebug() << "Item i" << m_i << "Item j: " << m_j;
        static auto pen = QPen(Qt::green, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        static auto blackPen = QPen(QColor("Black"));
        setPen(m_isActive ? pen : blackPen);

        if (m_isActive)
        {
            m_textItem = new QGraphicsTextItem("A");
            m_textItem->document()->setDocumentMargin(4);
            m_textItem->setDefaultTextColor(QColor("Red"));
            m_textItem->setPlainText(QObject::tr("A"));
            qDebug() << "Rect i" << m_rect.x() << "Rect j: " << m_rect.y();
            m_textItem->setX(boundingRect().center().x() - 150);
            m_textItem->setY(boundingRect().center().y() - 150);
            m_scene->addItem(m_textItem);

            QGraphicsSvgItem *item = new QGraphicsSvgItem("./images/icon.svg");

            QSvgRenderer *renderer = new QSvgRenderer(QStringLiteral("./images/icon.svg"));

            auto filename = QString("./images/icon.svg");
            QImage image(filename);
            image = image.scaled({30,30});

            m_pixmap = m_scene->addPixmap( QPixmap::fromImage(image));
            qDebug() << "Pos is: " << pos();
            m_pixmap->setPos(m_coords.center().x() - 15, m_coords.center().y() - 15);



            item->setSharedRenderer(renderer);
            item->setElementId(QStringLiteral("example"));
            m_textItem->setPos(boundingRect().center());
            m_scene->addItem(item);

        }
        else
        {
            m_scene->removeItem( m_textItem);
            m_scene->removeItem(m_pixmap);
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
    QGraphicsScene* m_scene;
    QGraphicsSvgItem* m_icon;
    QGraphicsPixmapItem * m_pixmap;
    QRectF m_coords;
};
