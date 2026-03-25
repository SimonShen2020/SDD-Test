// Module: Plugin/Null
// Responsibility boundary: Register null vendor creator into core factory.
// Dependency red line: Plugin may depend on Core abstractions but never vice versa.

#include "camera/core/CameraBase.hpp"
#include "camera/core/CameraFactory.hpp"
#include "camera/plugins/null/NullCamera.hpp"

#include <memory>

#if defined(_WIN32)
#define CAMERA_PLUGIN_EXPORT __declspec(dllexport)
#else
#define CAMERA_PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

extern "C" CAMERA_PLUGIN_EXPORT int RegisterCameraPlugin()
{
    camera::core::CoreResult result = camera::core::CameraFactory::Instance().Register("null",
        []()
        {
            return std::make_shared<camera::plugin_null::NullCamera>();
        });
    return result.ok() ? 0 : static_cast<int>(result.code);
}
