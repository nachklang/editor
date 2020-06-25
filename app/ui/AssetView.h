#pragma once

#include <QWidget>

#include <QHBoxLayout>
#include <QLabel>

namespace editor {

class AssetView: public QWidget
{
public:
    AssetView()
    {
        auto layout = new QHBoxLayout;
        auto label = new QLabel("ASSET VIEW WIDGET");
        layout->setAlignment(Qt::AlignHCenter);

        layout->addWidget(label);

        setLayout(layout);
    }

};

}
