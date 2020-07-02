#pragma once

#include <QJsonObject>
#include <QString>

#include <any>
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

    Range(){}

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

    Property(const QString& name, const QString& type, const std::any& value) :
      m_name(name), m_type(type), m_value(value)
    {
    }

protected:
    QString m_name;
    QString m_type;
    std::any m_value;

public:
    virtual void serialize() = 0;

    virtual void resetValue() = 0;

    virtual std::shared_ptr<Property> clone() = 0;

    QString name();

    QString type();

    std::any value();
};

std::shared_ptr<Property> createPropertyFromJson(const QJsonObject& object);

class BooleanProperty : public Property
{
public:
    BooleanProperty(const QString& name);

    void setValue(const bool& value);

    void resetValue() override;

    void serialize() override;

    std::shared_ptr<Property> clone() override;
};

class RangedDoubleProperty : public Property
{
public:
    RangedDoubleProperty(const QString& name, const Range<double>& range);

private:
    Range<double> m_range;

public:

    void setValue(const double& value);

    void resetValue() override
    {
        m_value.reset();
        m_value = m_range.m_min;
    }

    Range<double> range(){return m_range;}

    std::shared_ptr<Property> clone() override;

public:
    void serialize() override
    {
    }
};

} // namespace editor
