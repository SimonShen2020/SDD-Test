/**
 * @file test_factory_registration.cpp
 * @brief CameraFactory 工厂注册测试
 *
 * 验证 CameraFactory 的相机创建器注册功能：
 * - 正常注册相机创建器
 * - 拒绝重复注册同一厂商类型
 * - 返回正确的错误码 kAlreadyExists
 */

#include <gtest/gtest.h>

#include "camera/core/CameraFactory.hpp"

namespace
{

    class TestCamera final : public camera::core::ICamera
    {
      public:
        camera::core::CoreResult Initialize() override
        {
            return camera::core::CoreResult::Success();
        }
        camera::core::CoreResult Shutdown() override
        {
            return camera::core::CoreResult::Success();
        }
        camera::core::CoreResult Capture(camera::core::FrameData*) override
        {
            return camera::core::CoreResult::Success();
        }
        camera::core::CoreResult SetParameter(const std::string&, const std::string&) override
        {
            return camera::core::CoreResult::Success();
        }
        camera::core::CoreResult GetParameter(const std::string&, std::string*) const override
        {
            return camera::core::CoreResult::Success();
        }
    };

} // namespace

TEST(CameraFactoryTest, RejectsDuplicateRegistration)
{
    using camera::core::CameraFactory;
    using camera::core::CoreErrorCode;

    CameraFactory& factory = CameraFactory::Instance();
    auto first = factory.Register("dup_test_vendor",
        []()
        {
            return std::make_shared<TestCamera>();
        });
    auto second = factory.Register("dup_test_vendor",
        []()
        {
            return std::make_shared<TestCamera>();
        });

    EXPECT_TRUE(first.ok());
    EXPECT_FALSE(second.ok());
    EXPECT_EQ(second.code, CoreErrorCode::kAlreadyExists);
}
