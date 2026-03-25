// Module: Core
// Responsibility boundary: Poll camera health and drive reconnect attempts.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/core/CameraMonitor.hpp"

namespace camera
{
    namespace core
    {

        CameraMonitor::~CameraMonitor()
        {
            Stop();
        }

        void CameraMonitor::Start(const MonitorPolicy& policy, ProbeFn probe, ReconnectFn reconnect)
        {
            Stop();
            reconnectAttempts_.store(0);
            running_.store(true);

            worker_ = std::thread(
                [this, policy, probe, reconnect]()
                {
                    while (running_.load())
                    {
                        bool online = true;
                        if (probe)
                        {
                            online = probe();
                        }

                        if (!online)
                        {
                            int attempts = 0;
                            while (running_.load() && attempts < policy.maxReconnectAttempts)
                            {
                                ++attempts;
                                reconnectAttempts_.store(attempts);
                                if (reconnect && reconnect())
                                {
                                    break;
                                }
                                std::this_thread::sleep_for(policy.reconnectInterval);
                            }
                        }

                        std::this_thread::sleep_for(policy.monitorPeriod);
                    }
                });
        }

        void CameraMonitor::Stop()
        {
            running_.store(false);
            if (worker_.joinable())
            {
                worker_.join();
            }
        }

    } // namespace core
} // namespace camera
