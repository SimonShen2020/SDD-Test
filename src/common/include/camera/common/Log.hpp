#pragma once

#include <string>

namespace camera
{
    namespace common
    {

        void WriteLogLine(const std::string& level, const std::string& message);

    } // namespace common
} // namespace camera

#define CAMERA_LOG_INFO(msg) ::camera::common::WriteLogLine("INFO", (msg))
#define CAMERA_LOG_WARN(msg) ::camera::common::WriteLogLine("WARN", (msg))
#define CAMERA_LOG_ERROR(msg) ::camera::common::WriteLogLine("ERROR", (msg))
