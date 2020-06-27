#include "editor/ConfigReader.h"

#include <gtest/gtest.h>


TEST(TestConfigReader, Positive)
{
    auto reader = ConfigReader{};
    reader.readFromFile("readerTest.json");
}
