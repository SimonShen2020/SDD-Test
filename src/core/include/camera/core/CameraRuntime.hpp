#pragma once

#include <memory>
#include <string>
#include <vector>

#include "camera/core/CameraFactory.hpp"
#include "camera/core/CoreResult.hpp"

namespace camera
{
    namespace core
    {

        class CameraRuntime
        {
        public:
            CoreResult InitializePlugins(const std::vector<std::string>& pluginPaths, const std::string& registerSymbol);
            CoreResult CreateCamera(const std::string& vendorType, std::shared_ptr<ICamera>* outCamera) const;

        private:
            std::size_t m_loadedPlugins = 0;
        };

    } // namespace core
} // namespace camera
