#pragma once

#include "Actor.h"

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QPushButton>
#include <QStyleOptionGraphicsItem>
#include <QTextEdit>
#include "editor/Properties.h"

#include <QWidget>

#include <QGraphicsGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QGraphicsRectItem>

#include <QLabel>
#include <QVBoxLayout>
#include <memory>

namespace editor
{

class LevelView : public QWidget
{
    Q_OBJECT
public:
    LevelView()
    {
        auto layout = new QVBoxLayout;
        auto label = new QLabel("LEVEL VIEW WIDGET");
        layout->addWidget(label);

        auto view = new QGraphicsView;
        auto scene = new QGraphicsScene;

        auto createSize = []() {
            return QSize{actor::BORDER * level::FIELD_COLUMNS + 100,
                         actor::BORDER * level::FIELD_ROWS + 100};
        };

        auto viewSize = createSize();

        view->setFixedSize(viewSize);

        auto grid = new QGraphicsGridLayout;
        grid->setColumnFixedWidth(0, actor::BORDER);
        grid->setRowFixedHeight(0, actor::BORDER);

        auto mini = new QGraphicsWidget;
        mini->setMinimumHeight(actor::BORDER);
        mini->setMinimumWidth(actor::BORDER);

        mini->paint(new QPainter, new QStyleOptionGraphicsItem);
        grid->addItem(mini, 0, 1);

        auto activatedTracker =
            std::shared_ptr<ActivityTracker>(new ActivityTracker);
        QObject::connect(
            activatedTracker.get(),
            &ActivityTracker::activated,
            this,
            &LevelView::onActorActivated);

        for (auto i = 0; i < 10; ++i)
        {
            static auto a = 0.0;
            static auto b = 0.0;
            for (auto j = 0; j < 15; ++j)
            {

                static auto c = 30.0;
                static auto d = 30.0;

                grid->addItem(
                    new Actor(i, j, QRectF{a, b, c, d}, activatedTracker, scene),
                    i,
                    j);
                qDebug() << "i: " << i << "J: " << j;

                a += 30;
            }
            a = 0;
            b += 30;
        }

        scene->setSceneRect(0, 0, 14 * 30, 14 * 30);

        auto mainWindget = new QGraphicsWidget;
        mainWindget->setLayout(grid);

        scene->addItem(mainWindget);

        view->setScene(scene);
        layout->addWidget(view);

        setLayout(layout);
    }
public slots:
    void onActorActivated(Rected* index)
    {
        if (m_activatedActor.empty())
        {
            m_activatedActor.push_back(index);
        }
        else
        {

            if (m_activatedActor[0] != index)
            {
                m_activatedActor[0]->toggleActivated();
            }
            m_activatedActor.pop_back();

            if (m_activatedActor[0] != index)
            {
                m_activatedActor.push_back(index);
            }
        }
    }

private:
    std::vector<Rected*> m_activatedActor;
};

} // namespace editor
