#include <gtest/gtest.h>
#include "core/CameraService.h"
#include "core/CameraGroup.h"
#include <fstream>
#include <vector>

class E2EIntegrationTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Create an integration config reflecting a larger matrix
        std::ofstream ofs("e2e_config.json");
        ofs << R"({
          "vendor": "Null",
          "cameras": [
            { "name": "e2e_cam00" },
            { "name": "e2e_cam01" },
            { "name": "e2e_cam10" },
            { "name": "e2e_cam11" }
          ]
        })";
        ofs.close();
    }

    void TearDown() override
    {
        CAM::CameraService::instance().finalize();
        remove("e2e_config.json");
    }
};

TEST_F(E2EIntegrationTest, FullLifecycle)
{
    // 1. Initialize System
    bool initialized = CAM::CameraService::instance().initialize("e2e_config.json");
    ASSERT_TRUE(initialized) << "Camera subsystem failed to initialize end-to-end.";

    // 2. Retrieve Group
    auto group = CAM::CameraService::instance().getGroup();
    ASSERT_NE(group, nullptr) << "CameraGroup is null.";

    // 3. Define camera identifiers and manipulate
    std::vector<std::string> expected_cams = { "e2e_cam00", "e2e_cam01", "e2e_cam10", "e2e_cam11" };

    // 4. Batch Operations (Open & Start)
    for (const auto& name : expected_cams)
    {
        auto cam = group->getCamera(name);
        ASSERT_NE(cam, nullptr) << "Failed to resolve camera: " << name;

        EXPECT_FALSE(cam->isOpen());
        EXPECT_TRUE(cam->open());
        EXPECT_TRUE(cam->isOpen());
        EXPECT_TRUE(cam->startGrabbing());
    }

    // 5. System Interacts / Emulates Work
    // ... normal working emulation ...

    // 6. Batch Operations (Stop)
    for (const auto& name : expected_cams)
    {
        auto cam = group->getCamera(name);
        cam->stopGrabbing();
    }

    // 7. Test Close All via Group
    group->closeAll();

    for (const auto& name : expected_cams)
    {
        auto cam = group->getCamera(name);
        EXPECT_FALSE(cam->isOpen());
    }

    // 8. Finalize System (done in teardown, but explicit here as part of E2E verification)
    CAM::CameraService::instance().finalize();

    // Verify it's safe to call finalize multiple times
    EXPECT_NO_THROW(CAM::CameraService::instance().finalize());
}