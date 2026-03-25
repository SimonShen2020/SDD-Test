#pragma once

#include <string>

namespace camera
{
    namespace common
    {

        bool FileExists(const std::string& path);
        bool EnsureDirectory(const std::string& path);

    } // namespace common
} // namespace camera
