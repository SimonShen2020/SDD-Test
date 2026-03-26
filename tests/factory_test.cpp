#include <gtest/gtest.h>
#include "core/CameraFactory.h"
#include "plugins/null/NullCamera.h"

// Force dependency on CameraNull library so the Windows linker does not optimize it away,
// which ensures the DLL (and its static registration) is loaded.
void force_load_null_plugin()
{
    CAM::NullCamera dummy("dummy");
}

TEST(FactoryTest, CanCreateNullCamera)
{
    auto camera = CAM::CameraFactory::instance().createCamera("Null", "cam00");
    ASSERT_NE(camera, nullptr);
    auto null_camera = dynamic_cast<CAM::NullCamera*>(camera.get());
    EXPECT_NE(null_camera, nullptr);
}
