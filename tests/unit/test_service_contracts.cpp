/**
 * @file test_service_contracts.cpp
 * @brief 相机服务契约测试
 *
 * 验证 CameraService 的算法接口对空输入的处理是否符合契约：
 * - 所有算法接口应拒绝空帧数据
 * - 所有算法接口应返回 CoreResult 失败状态
 */

#include <gtest/gtest.h>

#include "camera/core/CameraService.hpp"

TEST(CameraServiceContractsTest, RejectsEmptyInputsWithCoreResult)
{
    camera::core::CameraService service;

    camera::core::FrameData empty;

    auto quality = service.RunQualityInspect(empty);
    auto splice = service.RunSplice(empty);
    auto distortion = service.RunDistortionCorrection(empty);
    auto locate = service.RunFourPointLocalization("");

    EXPECT_FALSE(quality.ok());
    EXPECT_FALSE(splice.ok());
    EXPECT_FALSE(distortion.ok());
    EXPECT_FALSE(locate.ok());
}
