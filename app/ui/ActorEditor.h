#pragma once

#include "Actor.h"
#include "ActorDisplayController.h"

#include "editor/ConfigReader.h"
#include "editor/Property.h"

#include <QComboBox>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>

#include <memory>

namespace editor
{

class ActorTypeWidget : public QWidget
{
    Q_OBJECT
public:
    ActorTypeWidget(const std::optional<Objects>& objects);

    bool hasValidType();

    QString currentText();

    void setCurrentIndex(const std::optional<Object>& object);
signals:
    void typeChanged(const std::optional<Object>& object);

private:
    QComboBox* m_comboBox;
    std::optional<Objects> m_objects;
};

class RangedDoubleWidget : public QWidget
{
    Q_OBJECT
public:
    RangedDoubleWidget(
        const std::shared_ptr<editor::RangedDoubleProperty>& property);

    void init();
public slots:
    void onChangedComplete()
    {
        m_property->setValue(m_spinBox->value());
    }

private:
    std::shared_ptr<editor::RangedDoubleProperty> m_property;
    QDoubleSpinBox* m_spinBox;
};

class BooleanWidget : public QWidget
{
    Q_OBJECT
public:
    BooleanWidget(const std::shared_ptr<editor::BooleanProperty>& property);

    void init();
public slots:
    void onChangedComplete()
    {
        m_property->setValue(
            m_comboBox->itemData(m_comboBox->currentIndex()).toBool());
    }

private:
    std::shared_ptr<editor::BooleanProperty> m_property;
    QComboBox* m_comboBox;
};

template <typename PropertyWidget, typename PropertyType>
void createPropertyWidget(
    QVBoxLayout* layout,
    QPushButton* saveButton,
    const std::shared_ptr<editor::Property>& property)
{
    auto propertyWidget =
        new PropertyWidget(std::static_pointer_cast<PropertyType>(property));
    propertyWidget->init();

    QObject::connect(
        saveButton,
        &QPushButton::clicked,
        propertyWidget,
        &PropertyWidget::onChangedComplete);

    layout->addWidget(propertyWidget);
}

class ActorEditor : public QWidget
{
    Q_OBJECT

    void createPropertiesWidgets(
        const std::optional<Properties>& properties,
        QVBoxLayout* sublayout,
        QPushButton* saveButton);

public:
    ActorEditor(const std::optional<Objects>& objects, std::shared_ptr<ActorDisplayController> displayController);

public slots:
    void receiveActiveActor(const std::shared_ptr<Actor>& actor);

    void onActorTypeChanged(const std::optional<Object>& object);



private:
    QVBoxLayout* m_mainLayout;
    QVBoxLayout* m_propertiesLayout;
    ActorTypeWidget* m_actorTypeWidget = nullptr;
    QPushButton* m_saveButton = nullptr;
    std::optional<Objects> m_objects;
    std::shared_ptr<ActorDisplayController> m_displayController;
    std::shared_ptr<Actor> m_activeActor;
};

} // namespace editor
