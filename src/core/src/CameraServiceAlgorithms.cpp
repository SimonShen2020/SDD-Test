// Module: Core
// Responsibility boundary: Service-side algorithm entry points with unified semantics.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/core/CameraService.hpp"

namespace camera
{
    namespace core
    {

        CoreResult CameraService::RunQualityInspect(const FrameData& frame)
        {
            if (frame.bytes.empty())
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "frame is empty");
            }
            return CoreResult::Success();
        }

        CoreResult CameraService::RunSplice(const FrameData& frame)
        {
            if (frame.bytes.empty())
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "frame is empty");
            }
            return CoreResult::Success();
        }

        CoreResult CameraService::RunDistortionCorrection(const FrameData& frame)
        {
            if (frame.bytes.empty())
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "frame is empty");
            }
            return CoreResult::Success();
        }

        CoreResult CameraService::RunFourPointLocalization(const std::string& imagePath)
        {
            if (imagePath.empty())
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "imagePath is empty");
            }
            return CoreResult::Success();
        }

    } // namespace core
} // namespace camera
