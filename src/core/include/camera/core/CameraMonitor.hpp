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
                return reconnectAttempts_.load();
            }

        private:
            std::atomic<bool> running_{ false };
            std::atomic<int> reconnectAttempts_{ 0 };
            std::thread worker_;
        };

    } // namespace core
} // namespace camera
