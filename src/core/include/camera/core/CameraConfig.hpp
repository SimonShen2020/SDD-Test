#pragma once

#include <string>
#include <vector>

namespace camera
{
    namespace core
    {

        struct CameraDescriptor
        {
            std::string userId;
            std::string deviceInternalId;
            int row = -1;
            int col = -1;
        };

        struct CameraConfig
        {
            std::string vendorType;
            int rows = 0;
            int cols = 0;
            std::vector<CameraDescriptor> cameras;
        };

    } // namespace core
} // namespace camera
