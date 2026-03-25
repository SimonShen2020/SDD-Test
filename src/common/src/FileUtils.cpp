// Module: Common
// Responsibility boundary: Provide file system utility functions.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/common/FileUtils.hpp"

#include <filesystem>

namespace camera
{
    namespace common
    {

        bool FileExists(const std::string& path)
        {
            std::error_code ec;
            return std::filesystem::exists(path, ec);
        }

        bool EnsureDirectory(const std::string& path)
        {
            std::error_code ec;
            if (std::filesystem::exists(path, ec))
            {
                return std::filesystem::is_directory(path, ec);
            }
            return std::filesystem::create_directories(path, ec);
        }

    } // namespace common
} // namespace camera
