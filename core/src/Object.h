#pragma once

#include <QString>

class Object
{
public:
    Object();

    QString name()
    {
        return m_name;
    }
    QString iconName()
    {
        return m_iconName;
    }

private:
    QString m_name;
    QString m_iconName;
    std::tuple<QString> m_properties;
};


