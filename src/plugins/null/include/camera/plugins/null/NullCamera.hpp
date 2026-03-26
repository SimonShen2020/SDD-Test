#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>

#include "camera/core/CameraBase.hpp"

namespace camera
{
    namespace plugin_null
    {

        // 模拟相机图像尺寸常量
        constexpr int kNullCameraWidth = 8;
        constexpr int kNullCameraHeight = 8;
        constexpr int kNullCameraPixelMaxValue = 255;

        class NullCamera final : public core::CameraBase
        {
        public:
            NullCamera();
            ~NullCamera() override = default;

            core::CoreResult Capture(core::FrameData* outFrame) override;
            core::CoreResult SetParameter(const std::string& key, const std::string& value) override;
            core::CoreResult GetParameter(const std::string& key, std::string* outValue) const override;

        private:
            core::CoreResult DoInitialize() override;
            core::CoreResult DoShutdown() override;

            mutable std::mutex m_mutex;
            std::unordered_map<std::string, std::string> m_parameters;
            std::atomic<unsigned int> m_frameCounter{ 0 };
        };

    } // namespace plugin_null
} // namespace camera
