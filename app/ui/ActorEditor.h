#pragma once

#include <QWidget>

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpacerItem>

namespace editor
{

class ActorEditor : public QWidget
{

public:
    ActorEditor()
    {
        auto layout = new QVBoxLayout;
        auto label = new QLabel("ACTOR EDITOR WIDGET");

        auto grid = new QGridLayout;
        auto typeLabel = new QLabel("Actor type:");
        auto typeCombobox = new QComboBox;

        typeCombobox->addItem("PlayerStart", QVariant(0));
        typeCombobox->addItem("Ladder", QVariant(1));
        typeCombobox->addItem("Gold", QVariant(2));

        grid->addWidget(typeLabel, 0,0);
        grid->addWidget(typeCombobox, 0,1);

        layout->addWidget(label);
        layout->addLayout(grid);
        layout->addItem(new QSpacerItem(0, 1000));

        setLayout(layout);
    }
};

} // namespace editor
