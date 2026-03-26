#include <gtest/gtest.h>
#include "core/CameraGroup.h"
#include "plugins/null/NullCamera.h"

TEST(CameraGroupTest, AddAndRetrieveCamera)
{
    CAM::CameraGroup group;
    group.addCamera("cam_test", std::make_unique<CAM::NullCamera>("cam_test"));

    EXPECT_NE(group.getCamera("cam_test"), nullptr);
    EXPECT_EQ(group.getCamera("cam_missing"), nullptr);
    EXPECT_EQ(group.getCamera("cam_test")->getName(), "cam_test");
}

TEST(CameraGroupTest, CloseAll)
{
    CAM::CameraGroup group;
    auto cam1 = std::make_unique<CAM::NullCamera>("cam1");
    auto cam2 = std::make_unique<CAM::NullCamera>("cam2");

    cam1->open();
    cam2->open();

    group.addCamera("cam1", std::move(cam1));
    group.addCamera("cam2", std::move(cam2));

    EXPECT_TRUE(group.getCamera("cam1")->isOpen());
    EXPECT_TRUE(group.getCamera("cam2")->isOpen());

    group.closeAll();

    EXPECT_FALSE(group.getCamera("cam1")->isOpen());
    EXPECT_FALSE(group.getCamera("cam2")->isOpen());
}