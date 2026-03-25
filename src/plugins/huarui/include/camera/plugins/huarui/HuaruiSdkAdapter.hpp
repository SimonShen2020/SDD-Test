#pragma once

#include "camera/core/CoreResult.hpp"
#include "camera/core/ICamera.hpp"

namespace camera
{
    namespace plugin_huarui
    {

        class HuaruiSdkAdapter
        {
        public:
            core::CoreResult Initialize();
            core::CoreResult Shutdown();
            core::CoreResult Capture(core::FrameData* outFrame);
        };

    } // namespace plugin_huarui
} // namespace camera
