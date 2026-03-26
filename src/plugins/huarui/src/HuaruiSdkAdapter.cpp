// Module: Plugin/Huarui
// Responsibility boundary: Adapt Huarui SDK calls behind stable plugin interface.
// Dependency red line: SDK headers and APIs must stay in plugin layer.

#include "camera/plugins/huarui/HuaruiSdkAdapter.hpp"

namespace camera
{
    namespace plugin_huarui
    {

        core::CoreResult HuaruiSdkAdapter::Initialize()
        {
            return core::CoreResult::Success();
        }

        core::CoreResult HuaruiSdkAdapter::Shutdown()
        {
            return core::CoreResult::Success();
        }

        core::CoreResult HuaruiSdkAdapter::Capture(core::FrameData* outFrame)
        {
            if (outFrame == nullptr)
            {
                return core::CoreResult::Failure(core::CoreErrorCode::kInvalidArgument, "outFrame is null");
            }
            outFrame->width = kHuaruiSimulatedWidth;
            outFrame->height = kHuaruiSimulatedHeight;
            outFrame->pixelFormat = "GRAY8";
            outFrame->bytes.assign(kHuaruiSimulatedPixelCount, kHuaruiSimulatedPixelValue);
            return core::CoreResult::Success();
        }

    } // namespace plugin_huarui
} // namespace camera
