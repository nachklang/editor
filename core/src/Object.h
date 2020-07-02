#pragma once

#include "Property.h"

#include <memory>
#include <optional>

#include <QString>

using Properties = std::vector<std::shared_ptr<editor::Property>>;

class Object
{
public:
    Object(
        QString name,
        QString iconName,
        bool singlePlaceable,
        std::optional<Properties> properties = std::nullopt) :
      m_name(name), m_iconName(iconName), m_singlePlaceable(singlePlaceable), m_properties(properties)
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
    bool singlePlaceable() const
    {
        return m_singlePlaceable;
    }

    std::optional<Properties> properties() const
    {
        return m_properties;
    }

    void setProperties(const std::optional<Properties>& properties)
    {
        m_properties = properties;
    }

    bool operator==(const Object& other)
    {
        return m_name == other.m_name &&
                m_iconName == other.m_iconName &&
                m_singlePlaceable == other.m_singlePlaceable &&
                m_properties == other.m_properties;;
    }

    friend bool operator==(const Object& lhs, const Object& rhs);

private:
    QString m_name;
    QString m_iconName;
    bool m_singlePlaceable;
    std::optional<Properties> m_properties;
};
