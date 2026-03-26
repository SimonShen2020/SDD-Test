#include <gtest/gtest.h>
#include "core/CameraService.h"
#include <fstream>

class ServiceCallTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        std::ofstream ofs("config.json");
        ofs << R"({
          "vendor": "Null",
          "cameras": [
            { "name": "cam00" }
          ]
        })";
        ofs.close();
        CAM::CameraService::instance().initialize("config.json");
    }

    void TearDown() override
    {
        CAM::CameraService::instance().finalize();
        remove("config.json");
    }
};

TEST_F(ServiceCallTest, CanCallCameraMethods)
{
    auto group = CAM::CameraService::instance().getGroup();
    ASSERT_NE(group, nullptr);
    auto cam = group->getCamera("cam00");
    ASSERT_NE(cam, nullptr);

    EXPECT_FALSE(cam->isOpen());
    cam->open();
    EXPECT_TRUE(cam->isOpen());

    EXPECT_TRUE(cam->startGrabbing());
    cam->stopGrabbing();

    cam->close();
    EXPECT_FALSE(cam->isOpen());
}
