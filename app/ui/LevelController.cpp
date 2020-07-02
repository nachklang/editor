#include "LevelController.h"

#include "editor/Property.h"

#include <QFileDialog>

#include "QJsonArray"
#include "QJsonDocument"
#include "QJsonObject"

namespace
{

QJsonDocument saveActorsToDocument(
    const std::map<std::shared_ptr<ActorProxy>, std::shared_ptr<Actor>>&
        actorsOnScene)
{
    if (actorsOnScene.size())
    {
        auto actors = QJsonArray{};

        for (const auto& [proxy, actor]: actorsOnScene)
        {
            auto actorObject = actor->object();
            if (actorObject)
            {
                auto object = QJsonObject{};
                auto position = QJsonArray{};
                position.push_back(actor->position().m_x);
                position.push_back(actor->position().m_y);

                object.insert("position", position);
                object.insert("typename", actorObject.value().name());

                auto actorProperties = actorObject.value().properties();
                if (actorProperties && actorProperties.value().size())
                {
                    auto propertiesJson = QJsonArray{};
                    for (const auto& property: actorProperties.value())
                    {
                        auto propertyJson = QJsonObject{};
                        auto value = property->value();

                        if (property->type()
                            == editor::property::type::BOOLEAN_TYPE)
                        {
                            propertyJson.insert("name", property->name());
                            propertyJson.insert(
                                "value", std::any_cast<bool>(value));
                        }
                        else if (
                            property->type()
                            == editor::property::type::RANGED_DOUBLE_TYPE)
                        {
                            propertyJson.insert("name", property->name());
                            propertyJson.insert(
                                "value", std::any_cast<double>(value));
                        }

                        propertiesJson.push_back(propertyJson);
                    }
                    object.insert("properties", propertiesJson);
                }
                actors.push_back(object);
            }
        }

        auto mainObject = QJsonObject{};
        mainObject.insert("level", actors);

        return QJsonDocument(mainObject);
    }
    else
    {
        return {};
    }
}

} // namespace

void LevelController::createNewLevel()
{
    if (!std::all_of(
            m_actorsOnScene.begin(),
            m_actorsOnScene.end(),
            [](const auto& element) { return !element.second->object(); }))
    {
        for (const auto& [proxy, actor]: m_actorsOnScene)
        {
            if (actor->object())
            {
                hideDestroyedActor(actor->coords());
                actor->setObject(std::nullopt);
            }
        }
    }
}

void LevelController::openExistedLevel()
{
    auto fileName = QFileDialog::getOpenFileName(
        nullptr, tr("Open level"), "", tr("Editor Level (*.json)"));

    if (fileName.isEmpty())
    {
        return;
    }

    auto file = QFile(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    auto jsonDocument = QJsonDocument{QJsonDocument::fromJson(file.readAll())};

    createNewLevel();

    auto jsonObject = jsonDocument.object();

    auto level = jsonObject["level"].toArray();

    for (const auto& objectAtLevel: level)
    {
        auto positionJson = objectAtLevel.toObject()["position"].toArray();
        auto position =
            Position{positionJson[0].toInt(), positionJson[1].toInt()};

        auto actorOnLevel = std::find_if(
            m_actorsOnScene.begin(),
            m_actorsOnScene.end(),
            [&position](const auto& element) {
                return element.second->position() == position;
            });

        auto object = std::optional<Object>{std::nullopt};

        if (actorOnLevel != m_actorsOnScene.end())
        {
            auto name = objectAtLevel.toObject()["typename"].toString();
            auto objectFromConfig = std::find_if(
                m_objects.begin(),
                m_objects.end(),
                [name](const auto& element) { return name == element.name(); });

            if (objectFromConfig != m_objects.end())
            {
                object = *objectFromConfig;
            }

            auto properties = std::optional<Properties>{std::nullopt};

            if (objectAtLevel.toObject().contains("properties"))
            {
                auto propertiesJson =
                    objectAtLevel.toObject()["properties"].toArray();
                for (const auto& propertyJson: propertiesJson)
                {
                    auto objectProperties = object.value().properties().value();

                    auto findedProperty = std::find_if(
                        objectProperties.begin(),
                        objectProperties.end(),
                        [&propertyJson](const auto& propertyElement) {
                            return propertyElement->name()
                                   == propertyJson.toObject()["name"].toString();
                        });
                    auto finded = *findedProperty;

                    if (findedProperty != objectProperties.end())
                    {
                        auto tmp = (*findedProperty)->clone();
                        (*findedProperty) = tmp;

                        if ((*findedProperty)->name()
                            == editor::property::type::BOOLEAN_TYPE)
                        {
                            auto casted = std::static_pointer_cast<
                                editor::BooleanProperty>(*findedProperty);
                            casted->setValue(
                                propertyJson.toObject()["value"].toBool());
                        }
                        else if (
                            (*findedProperty)->name()
                            == editor::property::type::RANGED_DOUBLE_TYPE)
                        {
                            auto casted = std::static_pointer_cast<
                                editor::RangedDoubleProperty>(*findedProperty);
                            casted->setValue(
                                propertyJson.toObject()["value"].toDouble());
                        }
                    }
                }
            }
            auto updatedActor = actorOnLevel->second;
            updatedActor->setObject(object);
            showLoadedActor(
                updatedActor->coords(),
                updatedActor->object()->iconName(),
                updatedActor->object()->name());
        }
    }
}

void LevelController::saveLevel()
{
    auto fileName = QFileDialog::getSaveFileName(
        nullptr, tr("Save level"), "", tr("Editor Level (*.json)"));

    if (m_actorsOnScene.empty())
    {
        return;
    }

    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        auto file = QFile{fileName};
        if (!file.open(QIODevice::WriteOnly))
        {
            return;
        }
        auto document = saveActorsToDocument(m_actorsOnScene);
        file.write(document.toJson());
    }
}

void LevelController::setActorsOnScene(
    const std::map<std::shared_ptr<ActorProxy>, std::shared_ptr<Actor>>&
        actorsOnScene)
{
    m_actorsOnScene = actorsOnScene;
}
