// Module: Plugin/Null
// Responsibility boundary: Simulate camera behavior without real hardware.
// Dependency red line: Plugin may depend on Core abstractions but never vice versa.

#include "camera/plugins/null/NullCamera.hpp"

#include <chrono>

namespace camera
{
    namespace plugin_null
    {

        NullCamera::NullCamera()
        {
            parameters_["exposure_mode"] = "auto";
        }

        core::CoreResult NullCamera::Capture(core::FrameData* outFrame)
        {
            if (outFrame == nullptr)
            {
                return core::CoreResult::Failure(core::CoreErrorCode::kInvalidArgument, "outFrame is null");
            }

            const unsigned int frameId = ++frameCounter_;
            outFrame->width = 8;
            outFrame->height = 8;
            outFrame->pixelFormat = "GRAY8";
            outFrame->bytes.assign(outFrame->width * outFrame->height, static_cast<std::uint8_t>(frameId % 255));
            outFrame->timestampNs =
                static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());

            return core::CoreResult::Success();
        }

        core::CoreResult NullCamera::SetParameter(const std::string& key, const std::string& value)
        {
            if (key.empty())
            {
                return core::CoreResult::Failure(core::CoreErrorCode::kInvalidArgument, "key is empty");
            }

            std::lock_guard<std::mutex> lock(mutex_);
            parameters_[key] = value;
            return core::CoreResult::Success();
        }

        core::CoreResult NullCamera::GetParameter(const std::string& key, std::string* outValue) const
        {
            if (outValue == nullptr)
            {
                return core::CoreResult::Failure(core::CoreErrorCode::kInvalidArgument, "outValue is null");
            }

            std::lock_guard<std::mutex> lock(mutex_);
            const auto it = parameters_.find(key);
            if (it == parameters_.end())
            {
                return core::CoreResult::Failure(core::CoreErrorCode::kNotFound, "parameter is not found");
            }

            *outValue = it->second;
            return core::CoreResult::Success();
        }

        core::CoreResult NullCamera::DoInitialize()
        {
            return core::CoreResult::Success();
        }

        core::CoreResult NullCamera::DoShutdown()
        {
            return core::CoreResult::Success();
        }

    } // namespace plugin_null
} // namespace camera
