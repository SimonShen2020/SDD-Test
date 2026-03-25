// Module: Core
// Responsibility boundary: Provide reusable camera lifecycle template behavior.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/core/CameraBase.hpp"

namespace camera
{
    namespace core
    {

        CoreResult CameraBase::Initialize()
        {
            const bool wasInitialized = initialized_.exchange(true);
            if (wasInitialized)
            {
                return CoreResult::Success();
            }

            CoreResult result = DoInitialize();
            if (!result.ok())
            {
                initialized_.store(false);
            }
            return result;
        }

        CoreResult CameraBase::Shutdown()
        {
            const bool wasInitialized = initialized_.exchange(false);
            if (!wasInitialized)
            {
                return CoreResult::Success();
            }
            return DoShutdown();
        }

    } // namespace core
} // namespace camera
