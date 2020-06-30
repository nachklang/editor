#include "ActorDisplayController.h"

#include <QDir>
#include <QHash>
#include <QCoreApplication>
#include <QDebug>

namespace
{

QString coordsToId(const QRectF& rect)
{
    return QString("%1,%2,%3,%4")
        .arg(rect.x())
        .arg(rect.y())
        .arg(rect.width())
        .arg(rect.height());
}
} // namespace

ActorDisplayController::ActorDisplayController(
    const std::shared_ptr<QGraphicsScene>& scene) :
  m_scene(scene)
{
}

void ActorDisplayController::showRepresentation(
    const QRectF coords,
    const std::optional<QString>& iconName,
    const std::optional<QString>& aliasName)
{
    auto id = coordsToId(coords);
    auto find = m_storage.find(id);

    if (find == m_storage.end())
    {
        auto data = ActorDisplayData{};
        if (iconName)
        {
            qDebug() << "Image name: " << iconName.value();

            QImage image(QString{"objects/"} + iconName.value());
            image = image.scaled({30, 30});

            auto icon = std::make_shared<QGraphicsPixmapItem>();
            icon->setPixmap(QPixmap::fromImage(image));
            m_scene->addItem(icon.get());
            icon->setPos(coords.center().x() - 15, coords.center().y() - 15);

                       qDebug() << "Icon scene position: " << icon->x() << "and" << icon->y();


            data.m_icon = IconData{iconName, icon};
            data.m_alias = AliasData{aliasName, nullptr};
        }
        else if (aliasName)
        {
            auto alias = std::make_shared<QGraphicsTextItem>();
            alias->setPlainText(aliasName.value());
            alias->setDefaultTextColor(QColor("Red"));
            m_scene->addItem(alias.get());
            alias->setPos(coords.center().x() - 15, coords.center().y() - 15);

            data.m_icon = IconData{iconName, nullptr};
            data.m_alias = AliasData{aliasName, alias};
        }
        m_storage[id] = data;
    }
}

void ActorDisplayController::hideRepresentation(const QRectF& coords)
{
    auto find = m_storage.find(coordsToId(coords));

    if (find != m_storage.end())
    {
        auto iconPtr = find->second.m_icon.second;
        auto aliasPtr = find->second.m_alias.second;

        if (iconPtr)
        {
            m_scene->removeItem(iconPtr.get());
            iconPtr.reset();
        }
        else if (aliasPtr)
        {
            m_scene->removeItem(aliasPtr.get());
            aliasPtr.reset();
        }
        m_storage.erase(find);
    }
}
