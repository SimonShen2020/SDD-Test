// Module: Plugin/Huarui
// Responsibility boundary: Camera implementation backed by HuaruiSdkAdapter.
// Dependency red line: Keep SDK details out of Core.

#include "camera/plugins/huarui/HuaruiCamera.hpp"

namespace camera
{
    namespace plugin_huarui
    {

        core::CoreResult HuaruiCamera::Capture(core::FrameData* outFrame)
        {
            return adapter_.Capture(outFrame);
        }

        core::CoreResult HuaruiCamera::SetParameter(const std::string& key, const std::string& value)
        {
            if (key.empty())
            {
                return core::CoreResult::Failure(core::CoreErrorCode::kInvalidArgument, "key is empty");
            }
            if (key == "exposure_mode")
            {
                exposureMode_ = value;
            }
            return core::CoreResult::Success();
        }

        core::CoreResult HuaruiCamera::GetParameter(const std::string& key, std::string* outValue) const
        {
            if (outValue == nullptr)
            {
                return core::CoreResult::Failure(core::CoreErrorCode::kInvalidArgument, "outValue is null");
            }
            if (key == "exposure_mode")
            {
                *outValue = exposureMode_;
                return core::CoreResult::Success();
            }
            return core::CoreResult::Failure(core::CoreErrorCode::kNotFound, "parameter not found");
        }

        core::CoreResult HuaruiCamera::DoInitialize()
        {
            return adapter_.Initialize();
        }

        core::CoreResult HuaruiCamera::DoShutdown()
        {
            return adapter_.Shutdown();
        }

    } // namespace plugin_huarui
} // namespace camera
