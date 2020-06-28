#pragma once

#include "editor/ConfigReader.h"
#include "editor/Property.h"

#include <QDebug>
#include <QComboBox>
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
    ActorTypeWidget(const std::optional<Objects>& objects) : m_objects(objects)
    {
        auto layout = new QHBoxLayout(this);
        layout->addWidget(new QLabel("Actor type: "));

        m_comboBox = new QComboBox;
        layout->addWidget(m_comboBox);

        m_comboBox->addItem("");
        if (objects)
        {
            for (const auto& object: objects.value())
            {
                m_comboBox->addItem(object.name());
            }
        }

        auto slot = [=](const QString& objectName) {
            auto predicate = [&objectName](const auto& element) {
                return objectName == element.name();
            };

            auto find = std::find_if(
                objects.value().begin(), objects.value().end(), predicate);

            emit typeChanged(
                find != objects.value().end() ? std::optional<Object>{*find} :
                                                std::nullopt);
        };

        QObject::connect(m_comboBox, &QComboBox::currentTextChanged, slot);
    }

    void setCurrentIndex(const std::optional<Object>& object)
    {
       m_comboBox->setCurrentIndex(object ? m_comboBox->findText(object.value().name()) : 0);
       qDebug() << "Object name is " << object.value().name();
       qDebug() << "Finded index is: " << m_comboBox->findText(object.value().name());
       qDebug() << "Current index is: " << m_comboBox->currentIndex();
    }
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
        const std::shared_ptr<editor::RangedDoubleProperty>& property) :
      m_property(property)
    {
    }

    void init()
    {
        auto layout = new QHBoxLayout;
        setLayout(layout);
        if (m_property)
        {
            auto label = new QLabel(m_property->name());
            layout->addWidget(label);

            m_spinBox = new QDoubleSpinBox;
            auto min = m_property->range().m_min;
            auto max = m_property->range().m_max;
            m_spinBox->setRange(min, max);
            m_spinBox->setToolTip(QString{"From %1 to %2"}.arg(min, max));
            layout->addWidget(m_spinBox);
        }
    }
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
    BooleanWidget(const std::shared_ptr<editor::BooleanProperty>& property) :
      m_property(property)
    {
    }

    void init()
    {
        m_comboBox = new QComboBox;
        m_comboBox->addItem(tr("true"), QVariant{true});
        m_comboBox->addItem(tr("false"), QVariant{false});

        if (m_property->value())
        {
            m_comboBox->setCurrentIndex(m_property.get() ? 0 : 1);
        }
    }
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
        QPushButton* saveButton)
    {
        if (properties)
        {
            for (const auto& property: properties.value())
            {
                if (property->type() == property::type::BOOLEAN_TYPE)
                {
                    createPropertyWidget<BooleanWidget, BooleanProperty>(
                        sublayout, saveButton, property);
                }
                else if (property->type() == property::type::RANGED_DOUBLE_TYPE)
                {
                    createPropertyWidget<
                        RangedDoubleWidget,
                        RangedDoubleProperty>(sublayout, saveButton, property);
                }
            }
        }
    }

public:
    ActorEditor(const std::optional<Objects>& objects) : m_objects(objects)
    {
        auto label = new QLabel("ACTOR EDITOR WIDGET");

        // createPropertiesWidgets(m_objects->value()m_properties,
        // m_saveButton);
        m_mainLayout = new QVBoxLayout;
        m_mainLayout->addWidget(label);
        setLayout(m_mainLayout);
    }

public slots:
    void receiveActivatedObject(
        bool hasActivatedObject, const std::optional<Object>& object)
    {
        qDebug() <<  "Receiving object!";

        if (hasActivatedObject)
        {
            if (m_actorTypeWidget)
            {
                delete m_actorTypeWidget;
            }

            // add actor activity widget
            m_actorTypeWidget = new ActorTypeWidget(m_objects);
            m_actorTypeWidget->setCurrentIndex(object);


            m_mainLayout->addWidget(m_actorTypeWidget);

            m_saveButton = new QPushButton("Save changes", m_actorTypeWidget);

            if (object && object.value().properties())
            {
                m_propertiesLayout = new QVBoxLayout;

                createPropertiesWidgets(
                    object.value().properties(),
                    m_propertiesLayout,
                    m_saveButton);

                m_mainLayout->addLayout(m_propertiesLayout);
            }

            m_mainLayout->addWidget(m_saveButton);
            // select current type in actorTypeWidget

            // delete all properties widgets
            // reinitialize widget
        }
        else
        {
            qDeleteAll(m_mainLayout->children());
        }
    }

    void actorTypeChanged(const std::optional<Object>& /*object*/)
    {
    }

private:
    QVBoxLayout* m_mainLayout;
    QVBoxLayout* m_propertiesLayout;
    ActorTypeWidget* m_actorTypeWidget = nullptr;
    QPushButton* m_saveButton;
    std::optional<Objects> m_objects;
};

} // namespace editor
