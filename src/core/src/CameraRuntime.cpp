// Module: Core
// Responsibility boundary: Runtime orchestration for plugin loading and camera creation.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/core/CameraRuntime.hpp"

#include "camera/common/Log.hpp"
#include "camera/core/PluginLoader.hpp"

namespace camera
{
    namespace core
    {

        CoreResult CameraRuntime::InitializePlugins(const std::vector<std::string>& pluginPaths, const std::string& registerSymbol)
        {
            PluginLoader loader;
            std::size_t loadedCount = 0;
            CoreResult result = loader.LoadAndRegister(pluginPaths, registerSymbol, &loadedCount);
            if (!result.ok())
            {
                CAMERA_LOG_ERROR("failed to initialize plugins: " + result.message);
                return result;
            }

            m_loadedPlugins = loadedCount;
            CAMERA_LOG_INFO("plugins initialized: " + std::to_string(m_loadedPlugins));
            return CoreResult::Success();
        }

        CoreResult CameraRuntime::CreateCamera(const std::string& vendorType, std::shared_ptr<ICamera>* outCamera) const
        {
            return CameraFactory::Instance().Create(vendorType, outCamera);
        }

    } // namespace core
} // namespace camera
