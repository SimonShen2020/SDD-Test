#include <gtest/gtest.h>
#include "core/CameraService.h"
#include "core/CameraGroup.h"
#include "core/ICamera.h"
#include <fstream>

// Test fixture for creating a dummy config file
class CameraServiceTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        std::ofstream configFile("test_config.json");
        configFile << R"({
            "layout": { "rows": 1, "cols": 1 },
            "vendor": "Null",
            "cameras": [
                { "name": "TestCam", "row": 0, "col": 0 }
            ],
            "reconnect": { "intervalSeconds": 5, "maxRetries": 10 }
        })";
        configFile.close();
    }

    void TearDown() override
    {
        remove("test_config.json");
    }
};

TEST_F(CameraServiceTest, Initialization)
{
    // Need to load the plugin library first. This is tricky in tests.
    // This test will likely fail without a mechanism to load plugins.
    bool initialized = CAM::CameraService::instance().initialize("test_config.json");
    ASSERT_TRUE(initialized);

    CAM::CameraGroup* group = CAM::CameraService::instance().getGroup();
    ASSERT_NE(group, nullptr);

    CAM::ICamera* cam = group->getCamera("TestCam");
    ASSERT_NE(cam, nullptr);
    EXPECT_EQ(cam->getVendorName(), "Null");
    EXPECT_EQ(cam->getName(), "TestCam");

    CAM::CameraService::instance().finalize();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
