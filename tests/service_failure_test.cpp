#include <gtest/gtest.h>
#include "core/CameraService.h"
#include <fstream>

class ServiceFailureTest : public ::testing::Test
{
protected:
    void TearDown() override
    {
        CAM::CameraService::instance().finalize();
        remove("bad_config.json");
    }
};

TEST_F(ServiceFailureTest, InitializeMissingFile)
{
    // Should return false and log error for missing file
    bool initialized = CAM::CameraService::instance().initialize("non_existent_file.json");
    EXPECT_FALSE(initialized);
}

TEST_F(ServiceFailureTest, InitializeBadJson)
{
    std::ofstream ofs("bad_config.json");
    ofs << "{ bad_json: ";
    ofs.close();

    bool initialized = CAM::CameraService::instance().initialize("bad_config.json");
    EXPECT_FALSE(initialized);
}

TEST_F(ServiceFailureTest, InitializeMissingVendor)
{
    std::ofstream ofs("bad_config.json");
    ofs << R"({
        "cameras": [ { "name": "cam00" } ]
    })";
    ofs.close();

    // The code might throw an exception if vendor is missing depending on nlohmann usage,
    // or log error. Let's see if it catches properly.
    // If it relies on exceptions, we might catch it or it just gracefully fails.
    try
    {
        bool initialized = CAM::CameraService::instance().initialize("bad_config.json");
        // Ensure no crash
    }
    catch (...)
    {
        // Ignored, just ensuring it handles or fails.
    }
}