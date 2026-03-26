#pragma once

#include <chrono>

namespace camera
{
    namespace core
    {

        // 监控策略默认值常量（毫秒）
        constexpr int kDefaultMonitorPeriodMs = 2000;
        constexpr int kDefaultReconnectIntervalMs = 1000;
        constexpr int kDefaultMaxReconnectAttempts = 10;

        struct MonitorPolicy
        {
            std::chrono::milliseconds monitorPeriod{ kDefaultMonitorPeriodMs };
            std::chrono::milliseconds reconnectInterval{ kDefaultReconnectIntervalMs };
            int maxReconnectAttempts = kDefaultMaxReconnectAttempts;
        };

    } // namespace core
} // namespace camera
