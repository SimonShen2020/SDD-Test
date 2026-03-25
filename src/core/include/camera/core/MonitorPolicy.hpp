#pragma once

#include <chrono>

namespace camera
{
    namespace core
    {

        struct MonitorPolicy
        {
            std::chrono::milliseconds monitorPeriod{ 2000 };
            std::chrono::milliseconds reconnectInterval{ 1000 };
            int maxReconnectAttempts = 10;
        };

    } // namespace core
} // namespace camera
