#pragma once

#include <string>

#include "camera/core/CameraConfig.hpp"
#include "camera/core/CameraLayout.hpp"
#include "camera/core/CoreResult.hpp"

namespace camera
{
    namespace core
    {

        class CameraConfigParser
        {
        public:
            CoreResult ParseFromFile(const std::string& filePath, CameraConfig* outConfig, CameraLayout* outLayout) const;
            CoreResult ParseFromString(const std::string& xmlText, CameraConfig* outConfig, CameraLayout* outLayout) const;
        };

    } // namespace core
} // namespace camera
