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

} // namespace editor
