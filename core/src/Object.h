#pragma once

#include "Property.h"

#include <optional>
#include <memory>


#include <QString>

using Properties = std::vector<std::shared_ptr<editor::Property>>;

class Object
{
public:
    Object(
        QString name,
        QString iconName,
        std::optional<Properties> properties = std::nullopt) :
      m_name(name), m_iconName(iconName), m_properties(properties)
    {
    }

    QString name() const
    {
        return m_name;
    }
    QString iconName() const
    {
        return m_iconName;
    }
    std::optional<Properties> properties() const
    {
        return m_properties;
    }

private:
    QString m_name;
    QString m_iconName;
    std::optional<Properties> m_properties;
};
