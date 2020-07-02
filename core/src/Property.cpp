#include "Property.h"

namespace editor
{

namespace {

std::shared_ptr<Property> CreateRangedDoubleProperty(const QJsonObject& object, const QString& name)
{
    if (object.contains("min") && object.contains("max"))
    {
        return std::make_shared<RangedDoubleProperty>(name,
            Range<double>(
                object.value("min").toDouble(),
                object.value("max").toDouble()));
    }
    else
    {
        return {};
    }
}

}

std::shared_ptr<Property> createPropertyFromJson(const QJsonObject &object)
{
    if (object.isEmpty())
    {
        return {};
    }
    else
    {
        if (object.contains("type") && object.contains("name"))
        {
            auto type = object.value("type");
            auto name = object.value("name");

            if (!type.isNull() && !name.isNull())
            {
                auto nameAsString = name.toString();

                if (type.toString() == property::type::BOOLEAN_TYPE)
                {
                    return std::make_shared<BooleanProperty>(nameAsString);
                }
                else if (type.toString() == property::type::RANGED_DOUBLE_TYPE)
                {
                    return CreateRangedDoubleProperty(object, nameAsString);
                }
            }
        }
    }

    return {};
}

QString Property::name()
{
    return m_name;
}

QString Property::type()
{
    return m_type;
}

std::any Property::value()
{
    return m_value;
}

BooleanProperty::BooleanProperty(const QString &name) :
    Property(name, property::type::BOOLEAN_TYPE, false)
{
}

void BooleanProperty::setValue(const bool &value)
{
    m_value.reset();
    m_value = value;
}

void BooleanProperty::resetValue()
{
    m_value.reset();
    m_value = false;
}

void BooleanProperty::serialize()
{
}

std::shared_ptr<Property> BooleanProperty::clone()
{
    auto shared = std::make_shared<BooleanProperty>(*this);
    return std::static_pointer_cast<Property>(shared);
}

RangedDoubleProperty::RangedDoubleProperty(const QString &name, const Range<double> &range) :
    Property(name, property::type::RANGED_DOUBLE_TYPE, std::any{}), m_range(range)
{
}

void RangedDoubleProperty::setValue(const double &value)
{
    m_value.reset();
    m_value = value;
}

std::shared_ptr<Property> RangedDoubleProperty::clone()
{
    auto shared = std::make_shared<RangedDoubleProperty>(*this);
    return std::static_pointer_cast<Property>(shared);
}

} // namespace editor
