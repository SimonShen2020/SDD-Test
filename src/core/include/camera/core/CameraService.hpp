#pragma once

#include <memory>
#include <string>
#include <vector>

#include "camera/core/CameraRuntime.hpp"
#include "camera/core/ICameraService.hpp"

namespace camera
{
    namespace core
    {

        class CameraService final : public ICameraService
        {
        public:
            CameraService() = default;
            ~CameraService() override = default;

            void SetPluginPaths(std::vector<std::string> pluginPaths);

            CoreResult Initialize(const std::string& configPath) override;
            CoreResult Shutdown() override;
            CoreResult CaptureOnce(const std::string& userId) override;

            CoreResult RunQualityInspect(const FrameData& frame);
            CoreResult RunSplice(const FrameData& frame);
            CoreResult RunDistortionCorrection(const FrameData& frame);
            CoreResult RunFourPointLocalization(const std::string& imagePath);

        private:
            CoreResult EnsureInitialized() const;

            CameraRuntime m_runtime;
            std::vector<std::string> m_pluginPaths;
            bool m_initialized = false;
        };

    } // namespace core
} // namespace camera
