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

    QString name()
    {
        return m_name;
    }

    QString type()
    {
        return m_type;
    }

    std::any value()
    {
        return m_value;
    }
};

std::shared_ptr<Property> createPropertyFromJson(const QJsonObject& object);

class BooleanProperty : public Property
{
public:
    BooleanProperty(const QString& name) :
      Property(name, property::type::BOOLEAN_TYPE, false)
    {
    }

//    bool value()
//    {
//        return m_value;
//    }
    void setValue(const bool& value)
    {
        m_value.reset();
        m_value = value;
    }

    void resetValue() override
    {
        m_value.reset();
        m_value = false;
    }

    void serialize() override
    {
    }

    std::shared_ptr<Property> clone() override
    {
        auto shared = std::make_shared<BooleanProperty>(*this);
        return std::static_pointer_cast<Property>(shared);
    }


//private:
//    bool m_value;
};

class RangedDoubleProperty : public Property
{
public:
    RangedDoubleProperty(const QString& name, const Range<double>& range) :
        Property(name, property::type::RANGED_DOUBLE_TYPE, std::any{}), m_range(range)
    {
    }

private:
    Range<double> m_range;

public:
//    double value()
//    {
//        return m_value;
//    }

    void setValue(const double& value)
    {
        m_value.reset();
        m_value = value;
    }

    void resetValue() override
    {
        m_value.reset();
        m_value = m_range.m_min;
    }

    Range<double> range(){return m_range;}

    std::shared_ptr<Property> clone() override
    {
        auto shared = std::make_shared<RangedDoubleProperty>(*this);
        return std::static_pointer_cast<Property>(shared);
    }

public:
    void serialize() override
    {
    }
};

} // namespace editor
