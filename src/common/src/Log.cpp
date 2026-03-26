// Module: Common
// Responsibility boundary: Provide thread-safe logging utility.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/common/Log.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>

namespace camera::common
{

    namespace
    {
        std::mutex& LogMutex()
        {
            static std::mutex mutex;
            return mutex;
        }
    } // namespace

    void WriteLogLine(const std::string& level, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(LogMutex());

        std::ofstream logFile("camera.log", std::ios::app);
        if (!logFile.is_open())
        {
            return;
        }

        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

        std::tm tmValue{};
#if defined(_WIN32)
        localtime_s(&tmValue, &nowTime);
#else
        localtime_r(&nowTime, &tmValue);
#endif

        logFile << std::put_time(&tmValue, "%Y-%m-%d %H:%M:%S") << " [" << level << "] " << message << '\n';
    }

} // namespace camera::common
