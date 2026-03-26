#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

#include "camera/core/MonitorPolicy.hpp"

namespace camera
{
    namespace core
    {

        class CameraMonitor
        {
        public:
            using ProbeFn = std::function<bool()>;
            using ReconnectFn = std::function<bool()>;

            CameraMonitor() = default;
            ~CameraMonitor();

            void Start(const MonitorPolicy& policy, ProbeFn probe, ReconnectFn reconnect);
            void Stop();

            int reconnectAttempts() const
            {
                return m_reconnectAttempts.load();
            }

        private:
            std::atomic<bool> m_running{ false };
            std::atomic<int> m_reconnectAttempts{ 0 };
            std::thread m_worker;
        };

    } // namespace core
} // namespace camera
