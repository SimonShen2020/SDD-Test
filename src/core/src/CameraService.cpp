// Module: Core
// Responsibility boundary: Expose top-level camera service orchestration.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/core/CameraService.hpp"

#include <utility>

#include "camera/core/PluginEntry.hpp"

namespace camera
{
    namespace core
    {

        void CameraService::SetPluginPaths(std::vector<std::string> pluginPaths)
        {
            m_pluginPaths = std::move(pluginPaths);
        }

        CoreResult CameraService::Initialize(const std::string& configPath)
        {
            (void)configPath;

            CoreResult result = m_runtime.InitializePlugins(m_pluginPaths, kDefaultPluginRegisterSymbol);
            if (!result.ok())
            {
                return result;
            }

            m_initialized = true;
            return CoreResult::Success();
        }

        CoreResult CameraService::Shutdown()
        {
            m_initialized = false;
            return CoreResult::Success();
        }

        CoreResult CameraService::CaptureOnce(const std::string& userId)
        {
            CoreResult guard = EnsureInitialized();
            if (!guard.ok())
            {
                return guard;
            }

            std::shared_ptr<ICamera> camera;
            CoreResult createResult = m_runtime.CreateCamera(userId, &camera);
            if (!createResult.ok())
            {
                return createResult;
            }

            FrameData frame;
            return camera->Capture(&frame);
        }

        CoreResult CameraService::EnsureInitialized() const
        {
            if (!m_initialized)
            {
                return CoreResult::Failure(CoreErrorCode::kRuntimeError, "service is not initialized");
            }
            return CoreResult::Success();
        }

    } // namespace core
} // namespace camera
