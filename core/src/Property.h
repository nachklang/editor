#pragma once

#include <QJsonObject>
#include <QString>

#include <memory>

namespace editor
{

namespace property::type
{

enum class PropertyType
{
    /*INT,
    DOUBLE,*/
    BOOLEAN,
    /*RANGED_INT,*/
    RANGED_DOUBLE,
    /*STRING,
    LIST*/
};

const auto BOOLEAN_TYPE = "boolean";
const auto RANGED_DOUBLE_TYPE = "ranged_double";

} // namespace property::type

template <typename T>
struct Range
{
    T m_min;
    T m_max;

    Range(double min, double max) : m_min(std::move(min)), m_max(std::move(max))
    {
    }
};

class Property
{
public:
    ~Property()
    {
    }

    Property(const QString& name, const QString& type) :
      m_name(name), m_type(type)
    {
    }

protected:
    QString m_name;
    QString m_type;

public:
    virtual void serialize() = 0;

    QString name()
    {
        return m_name;
    }

    QString type()
    {
        return m_type;
    }
};

std::shared_ptr<Property> createPropertyFromJson(const QJsonObject& object);

class BooleanProperty : public Property
{
public:
    BooleanProperty(const QString& name) :
      Property(name, property::type::BOOLEAN_TYPE)
    {
    }

private:
    bool m_value;

public:
    bool value()
    {
        return m_value;
    }
    void setValue(const bool& value)
    {
        m_value = value;
    }

    void serialize() override
    {
    }
};

class RangedDoubleProperty : public Property
{
public:
    RangedDoubleProperty(const QString& name, const Range<double>& range) :
      Property(name, property::type::RANGED_DOUBLE_TYPE), m_range(range)
    {
    }

private:
    Range<double> m_range;
    double m_value;

public:
    double value();
    void setValue(const double& value)
    {
        m_value = value;
    }

    Range<double> range(){return m_range;}

public:
    void serialize() override
    {
    }
};

} // namespace editor
