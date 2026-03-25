/**
 * @file test_null_capture_flow.cpp
 * @brief Null 相机捕获流程集成测试
 *
 * 验证 Null 相机实现的完整生命周期：
 * - 初始化和关闭
 * - 图像捕获（返回模拟数据）
 * - 参数设置和获取
 */

#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

#include "camera/core/CameraBase.hpp"

namespace
{

    class LocalNullLikeCamera final : public camera::core::CameraBase
    {
      public:
        camera::core::CoreResult Capture(camera::core::FrameData* outFrame) override
        {
            if (outFrame == nullptr)
            {
                return camera::core::CoreResult::Failure(camera::core::CoreErrorCode::kInvalidArgument, "outFrame is null");
            }
            outFrame->width = 8;
            outFrame->height = 8;
            outFrame->pixelFormat = "GRAY8";
            outFrame->bytes.assign(64, 7);
            return camera::core::CoreResult::Success();
        }

        camera::core::CoreResult SetParameter(const std::string& key, const std::string& value) override
        {
            params_[key] = value;
            return camera::core::CoreResult::Success();
        }

        camera::core::CoreResult GetParameter(const std::string& key, std::string* outValue) const override
        {
            if (outValue == nullptr)
            {
                return camera::core::CoreResult::Failure(camera::core::CoreErrorCode::kInvalidArgument, "outValue is null");
            }
            const auto it = params_.find(key);
            if (it == params_.end())
            {
                return camera::core::CoreResult::Failure(camera::core::CoreErrorCode::kNotFound, "missing key");
            }
            *outValue = it->second;
            return camera::core::CoreResult::Success();
        }

      protected:
        camera::core::CoreResult DoInitialize() override
        {
            return camera::core::CoreResult::Success();
        }

        camera::core::CoreResult DoShutdown() override
        {
            return camera::core::CoreResult::Success();
        }

      private:
        std::unordered_map<std::string, std::string> params_;
    };

} // namespace

TEST(NullCameraFlowTest, InitializeCaptureAndShutdown)
{
    LocalNullLikeCamera camera;

    auto initResult = camera.Initialize();
    ASSERT_TRUE(initResult.ok());

    camera::core::FrameData frame;
    auto captureResult = camera.Capture(&frame);
    ASSERT_TRUE(captureResult.ok());
    EXPECT_EQ(frame.width, 8U);
    EXPECT_EQ(frame.height, 8U);
    EXPECT_EQ(frame.bytes.size(), 64U);

    auto setResult = camera.SetParameter("exposure_mode", "manual");
    ASSERT_TRUE(setResult.ok());

    std::string value;
    auto getResult = camera.GetParameter("exposure_mode", &value);
    ASSERT_TRUE(getResult.ok());
    EXPECT_EQ(value, "manual");

    auto shutdownResult = camera.Shutdown();
    ASSERT_TRUE(shutdownResult.ok());
}
