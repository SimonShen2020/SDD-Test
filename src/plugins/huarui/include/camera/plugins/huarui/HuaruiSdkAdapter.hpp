#pragma once

#include "camera/core/CoreResult.hpp"
#include "camera/core/ICamera.hpp"

namespace camera
{
    namespace plugin_huarui
    {

        // 模拟图像尺寸常量
        constexpr int kHuaruiSimulatedWidth = 4;
        constexpr int kHuaruiSimulatedHeight = 4;
        constexpr int kHuaruiSimulatedPixelCount = kHuaruiSimulatedWidth * kHuaruiSimulatedHeight;
        constexpr std::uint8_t kHuaruiSimulatedPixelValue = 128;

        class HuaruiSdkAdapter
        {
        public:
            core::CoreResult Initialize();
            core::CoreResult Shutdown();
            core::CoreResult Capture(core::FrameData* outFrame);
        };

    } // namespace plugin_huarui
} // namespace camera
