/**
 * @file test_camera_layout.cpp
 * @brief CameraLayout 布局管理测试
 *
 * 验证 CameraLayout 的相机矩阵拓扑管理功能：
 * - 3x8 布局的正常配置和相机添加
 * - 越界位置和重复添加的拒绝
 * - 无效布局尺寸和 userId 格式的拒绝
 */

#include <gtest/gtest.h>

#include "camera/core/CameraLayout.hpp"

TEST(CameraLayoutTest, AcceptsValid3x8Topology)
{
    camera::core::CameraLayout layout;
    auto dimResult = layout.SetDimensions(3, 8);
    ASSERT_TRUE(dimResult.ok());

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            camera::core::CameraSlot slot;
            slot.row = r;
            slot.col = c;
            slot.userId = "DHA" + std::string(1, static_cast<char>('A' + r)) + std::to_string(c);
            slot.deviceInternalId = "null-" + std::to_string(r * 8 + c);
            auto addResult = layout.AddCamera(slot);
            ASSERT_TRUE(addResult.ok());
        }
    }

    EXPECT_EQ(layout.cameraCount(), 24U);

    std::string userId;
    auto queryResult = layout.GetUserId(2, 7, &userId);
    ASSERT_TRUE(queryResult.ok());
    EXPECT_EQ(userId, "DHAC7");
}

TEST(CameraLayoutTest, RejectsOutOfBoundAndDuplicate)
{
    camera::core::CameraLayout layout;
    ASSERT_TRUE(layout.SetDimensions(2, 2).ok());

    camera::core::CameraSlot slot;
    slot.userId = "DHAA0";
    slot.row = 0;
    slot.col = 0;
    ASSERT_TRUE(layout.AddCamera(slot).ok());

    camera::core::CameraSlot duplicate = slot;
    auto duplicateResult = layout.AddCamera(duplicate);
    EXPECT_FALSE(duplicateResult.ok());

    camera::core::CameraSlot oob;
    oob.userId = "DHAB9";
    oob.row = 5;
    oob.col = 0;
    auto oobResult = layout.AddCamera(oob);
    EXPECT_FALSE(oobResult.ok());
}

TEST(CameraLayoutTest, RejectsInvalidLayoutSizeAndUserId)
{
    camera::core::CameraLayout layout;
    auto dimResult = layout.SetDimensions(0, 11);
    EXPECT_FALSE(dimResult.ok());

    ASSERT_TRUE(layout.SetDimensions(1, 1).ok());
    camera::core::CameraSlot badId;
    badId.userId = "bad-id";
    badId.row = 0;
    badId.col = 0;
    auto addResult = layout.AddCamera(badId);
    EXPECT_FALSE(addResult.ok());
}
