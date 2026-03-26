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
            return m_adapter.Capture(outFrame);
        }

        core::CoreResult HuaruiCamera::SetParameter(const std::string& key, const std::string& value)
        {
            if (key.empty())
            {
                return core::CoreResult::Failure(core::CoreErrorCode::kInvalidArgument, "key is empty");
            }
            if (key == "exposure_mode")
            {
                m_exposureMode = value;
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
                *outValue = m_exposureMode;
                return core::CoreResult::Success();
            }
            return core::CoreResult::Failure(core::CoreErrorCode::kNotFound, "parameter not found");
        }

        core::CoreResult HuaruiCamera::DoInitialize()
        {
            return m_adapter.Initialize();
        }

        core::CoreResult HuaruiCamera::DoShutdown()
        {
            return m_adapter.Shutdown();
        }

    } // namespace plugin_huarui
} // namespace camera
