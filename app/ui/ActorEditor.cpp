#include "ActorEditor.h"

namespace editor
{

ActorTypeWidget::ActorTypeWidget(const std::optional<Objects> &objects) :
  m_objects(objects)
{
    auto layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("Actor type: "));

    m_comboBox = new QComboBox;
    layout->addWidget(m_comboBox);

    m_comboBox->addItem("");
    if (objects)
    {
        for (const auto &object: objects.value())
        {
            m_comboBox->addItem(object.name());
        }
    }

    auto slot = [=](const QString &objectName) {
        auto predicate = [&objectName](const auto &element) {
            return objectName == element.name();
        };

        auto find = std::find_if(
            objects.value().begin(), objects.value().end(), predicate);

        auto objectToSend = find != objects.value().end() ?
                                std::optional<Object>{*find} :
                                std::nullopt;

        emit typeChanged(objectToSend);
    };

    QObject::connect(m_comboBox, &QComboBox::currentTextChanged, slot);
}

bool ActorTypeWidget::hasValidType()
{
    return m_comboBox->currentIndex() != 0;
}

QString ActorTypeWidget::currentText()
{
    return m_comboBox->currentText();
}

void ActorTypeWidget::setCurrentIndex(const std::optional<Object> &object)
{
    m_comboBox->setCurrentIndex(
        object ? m_comboBox->findText(object.value().name()) : 0);
    //       qDebug() << "Object name is " << object.value().name();
    //       qDebug() << "Finded index is: " <<
    //       m_comboBox->findText(object.value().name()); qDebug() << "Current
    //       index is: " << m_comboBox->currentIndex();
}

RangedDoubleWidget::RangedDoubleWidget(
    const std::shared_ptr<RangedDoubleProperty> &property) :
  m_property(property)
{
}

void RangedDoubleWidget::init()
{
    auto layout = new QHBoxLayout;

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
    setLayout(layout);
}

BooleanWidget::BooleanWidget(const std::shared_ptr<BooleanProperty> &property) :
  m_property(property)
{
}

void BooleanWidget::init()
{
    auto layout = new QHBoxLayout;

    auto label = new QLabel(m_property->name());
    layout->addWidget(label);

    m_comboBox = new QComboBox;
    m_comboBox->addItem(tr("true"), QVariant{true});
    m_comboBox->addItem(tr("false"), QVariant{false});

    if (m_property->value())
    {
        m_comboBox->setCurrentIndex(m_property.get() ? 0 : 1);
    }
    layout->addWidget(m_comboBox);
    setLayout(layout);
}

void ActorEditor::createPropertiesWidgets(
    const std::optional<Properties> &properties,
    QVBoxLayout *sublayout,
    QPushButton *saveButton)
{
    if (properties)
    {
        for (const auto &property: properties.value())
        {
            if (property->type() == property::type::BOOLEAN_TYPE)
            {
                createPropertyWidget<BooleanWidget, BooleanProperty>(
                    sublayout, saveButton, property);
            }
            else if (property->type() == property::type::RANGED_DOUBLE_TYPE)
            {
                createPropertyWidget<RangedDoubleWidget, RangedDoubleProperty>(
                    sublayout, saveButton, property);
            }
        }
    }
}

ActorEditor::ActorEditor(
    const std::optional<Objects> &objects,
    const std::shared_ptr<ActorDisplayController> displayController) :
  m_objects(objects), m_displayController(displayController)
{
    if (objects)
    {
        qDebug() << "OBJECTS SIZE IN ACTOR_EDITOR" << objects.value().size();
    }
    auto label = new QLabel("ACTOR EDITOR WIDGET");
    m_actorTypeWidget = new ActorTypeWidget(objects);
    m_actorTypeWidget->hide();

    QObject::connect(
        m_actorTypeWidget,
        &ActorTypeWidget::typeChanged,
        this,
        &ActorEditor::onActorTypeChanged);

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->addWidget(label);
    m_mainLayout->addWidget(m_actorTypeWidget);
    setLayout(m_mainLayout);
}

void ActorEditor::receiveActiveActor(const std::shared_ptr<Actor> &actor)
{
    qDebug() << "Receiving object!";
    m_activeActor = actor;

    if (actor)
    {
        auto object = actor->object();
        if (!m_actorTypeWidget)
        {
            // delete m_actorTypeWidget;
            m_actorTypeWidget = new ActorTypeWidget(m_objects);
            m_mainLayout->addWidget(m_actorTypeWidget);
        }
        else
        {
            m_actorTypeWidget->show();
        }

        m_actorTypeWidget->setCurrentIndex(object);

        if (!m_saveButton)
        {
            m_saveButton = new QPushButton("Save changes", m_actorTypeWidget);
            m_mainLayout->addWidget(m_saveButton);
            QObject::connect(
                m_saveButton, &QPushButton::clicked, this, [this]() {
                    if (m_actorTypeWidget->hasValidType())
                    {
                        auto it = std::find_if(
                            m_objects.value().begin(),
                            m_objects.value().end(),
                            [this](const auto &element) {
                                return m_actorTypeWidget->currentText()
                                       == element.name();
                            });

                        if (it != m_objects.value().end())
                        {
                            m_displayController->showRepresentation(
                                m_activeActor->coords(),
                                it->iconName()
                                        ,
                                it->name());
                        }
                    }
                    else
                    {
                        m_displayController->hideRepresentation(
                            m_activeActor->coords());
                    }
                });
        }

        if (object && object.value().properties())
        {
            m_propertiesLayout = new QVBoxLayout;

            createPropertiesWidgets(
                object.value().properties(), m_propertiesLayout, m_saveButton);

            m_mainLayout->addLayout(m_propertiesLayout);
        }
    }
    else
    {
        delete m_saveButton;
        qDeleteAll(m_mainLayout->children());
    }
}

void ActorEditor::onActorTypeChanged(const std::optional<Object> &object)
{
    if (object)
    {
        qDebug() << "ActorEditor::onActorTypeChanged(): "
                 << "name: " << object.value().name();
    }

    if (object && object.value().properties())
    {
                    if(m_propertiesLayout)
                    {
                        qDeleteAll(m_propertiesLayout->children());
                        delete m_propertiesLayout;
                        m_propertiesLayout = new QVBoxLayout;
                        m_mainLayout->addLayout(m_propertiesLayout);
                    }
                    else
                    {
        m_propertiesLayout = new QVBoxLayout;

                    }

        createPropertiesWidgets(
            object.value().properties(), m_propertiesLayout, m_saveButton);

        m_mainLayout->addLayout(m_propertiesLayout);
    }
}

} // namespace editor
