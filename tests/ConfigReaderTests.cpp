#include "editor/ConfigReader.h"
#include "editor/Property.h"

#include <string>

#include <iostream>

#include <gtest/gtest.h>

namespace {

std::string toStd(const QString& string)
{
return string.toStdString();
}
}


TEST(TestConfigReader, Positive)
{
    auto reader = ConfigReader{};
    reader.readFromFile("readerTest.json");
}

TEST(ReadPropertiesFromFile, Positive)
{
    constexpr auto TEST_FILE_NAME = "readProperty.json";

    auto reader = ConfigReader{};
    auto object = reader.createJsonObjectFromFile(TEST_FILE_NAME);

    auto properties = reader.readTypeProperties(object);

    EXPECT_EQ(properties->size(), 2);

    auto boolean = properties.value()[0];
    EXPECT_EQ(toStd(boolean->name()), "TestBooleanProperty");
    EXPECT_EQ(toStd(boolean->type()), editor::property::type::BOOLEAN_TYPE);

    auto rangedDouble = properties.value()[1];
    EXPECT_EQ(toStd(rangedDouble->name()), "TestRangedDoubleProperty");
    EXPECT_EQ(toStd(rangedDouble->type()), editor::property::type::RANGED_DOUBLE_TYPE);
    auto ranged = std::static_pointer_cast<editor::RangedDoubleProperty>(rangedDouble);
    EXPECT_EQ(ranged->range().m_min, 0.01);
    EXPECT_EQ(ranged->range().m_max, 1);
}

TEST(ReadTypesFromFile, Positive)
{
    constexpr auto TEST_FILE_NAME = "test_objects.json";

    auto reader = ConfigReader{};
    auto object = reader.createJsonObjectFromFile(TEST_FILE_NAME);

    auto types = reader.readObjectsFromJson(object);

    EXPECT_EQ(types.size(), 3);

    EXPECT_EQ(toStd(types[0].name()), "TestTypeWithoutProperties");
    EXPECT_EQ(toStd(types[0].iconName()), "without.png");
    EXPECT_FALSE(types[0].properties());

    EXPECT_EQ(toStd(types[1].name()), "TypeWithEmptyProperties");
    EXPECT_EQ(toStd(types[1].iconName()), "without2.png");
    EXPECT_FALSE(types[1].properties());

    std::cout << "IS INITIALIZED" << types[1].properties().has_value();

    EXPECT_EQ(toStd(types[2].name()), "TypeWithOneProperty");
    EXPECT_EQ(toStd(types[2].iconName()), "one.png");
    EXPECT_TRUE(types[2].properties());
    EXPECT_EQ(types[2].properties()->size(),1);
    EXPECT_EQ(toStd(types[2].properties().value()[0]->name()), "BooleanProperty");
    EXPECT_EQ(toStd(types[2].properties().value()[0]->type()), editor::property::type::BOOLEAN_TYPE);
}
