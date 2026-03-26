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
            m_reconnectAttempts.store(0);
            m_running.store(true);

            m_worker = std::thread(
                [this, policy, probe, reconnect]()
                {
                    while (m_running.load())
                    {
                        bool online = true;
                        if (probe)
                        {
                            online = probe();
                        }

                        if (!online)
                        {
                            int attempts = 0;
                            while (m_running.load() && attempts < policy.maxReconnectAttempts)
                            {
                                ++attempts;
                                m_reconnectAttempts.store(attempts);
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
            m_running.store(false);
            if (m_worker.joinable())
            {
                m_worker.join();
            }
        }

    } // namespace core
} // namespace camera
