#include <gtest/gtest.h>
#include "core/CameraService.h"
#include <fstream>

class ServiceLayoutTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create a dummy config file for testing
        std::ofstream ofs("config.json");
        ofs << R"({
          "vendor": "Null",
          "cameras": [
            { "name": "cam00" },
            { "name": "cam01" },
            { "name": "cam10" },
            { "name": "cam11" }
          ]
        })";
        ofs.close();
    }

    void TearDown() override
    {
        remove("config.json");
    }
};

TEST_F(ServiceLayoutTest, CanParseConfigAndCreateLayout)
{
    bool initialized = CAM::CameraService::instance().initialize("config.json");
    ASSERT_TRUE(initialized);

    auto group = CAM::CameraService::instance().getGroup();
    ASSERT_NE(group, nullptr);

    ASSERT_NE(group->getCamera("cam00"), nullptr);
    ASSERT_NE(group->getCamera("cam01"), nullptr);
    ASSERT_NE(group->getCamera("cam10"), nullptr);
    ASSERT_NE(group->getCamera("cam11"), nullptr);

    EXPECT_EQ(group->getCamera("cam_none"), nullptr);

    CAM::CameraService::instance().finalize();
}
