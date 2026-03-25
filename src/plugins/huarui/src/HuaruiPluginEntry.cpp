// Module: Plugin/Huarui
// Responsibility boundary: Register Huarui camera creator in core factory.
// Dependency red line: Plugin may depend on Core abstractions but never vice versa.

#include "camera/core/CameraFactory.hpp"
#include "camera/plugins/huarui/HuaruiCamera.hpp"

#include <memory>

#if defined(_WIN32)
#define CAMERA_PLUGIN_EXPORT __declspec(dllexport)
#else
#define CAMERA_PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

extern "C" CAMERA_PLUGIN_EXPORT int RegisterCameraPlugin()
{
    auto result = camera::core::CameraFactory::Instance().Register("huarui",
        []()
        {
            return std::make_shared<camera::plugin_huarui::HuaruiCamera>();
        });
    return result.ok() ? 0 : static_cast<int>(result.code);
}
