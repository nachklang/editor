#pragma once

#include "editor/Object.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <memory>
#include <optional>

#include "Property.h"

using Properties = std::vector<std::shared_ptr<editor::Property>>;
using Objects = std::vector<Object>;

class ConfigReader
{
public:
    void readFromFile(const QString& fileName);

    std::optional<Properties> readTypeProperties(
        const QJsonObject& object);

    Objects readObjectsFromJson(const QJsonObject& object);

    QJsonObject createJsonObjectFromFile(const QString& fileName);
};
