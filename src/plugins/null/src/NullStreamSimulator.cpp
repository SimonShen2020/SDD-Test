// Module: Plugin/Null
// Responsibility boundary: Simulate asynchronous frame callback behavior.
// Dependency red line: Plugin may depend on Core abstractions but never vice versa.

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

namespace camera
{
    namespace plugin_null
    {

        using FrameCallback = std::function<void()>;

        class NullStreamSimulator
        {
        public:
            ~NullStreamSimulator()
            {
                Stop();
            }

            void Start(FrameCallback callback, std::chrono::milliseconds period)
            {
                Stop();
                m_running.store(true);
                m_worker = std::thread(
                    [this, callback, period]()
                    {
                        while (m_running.load())
                        {
                            if (callback)
                            {
                                callback();
                            }
                            std::this_thread::sleep_for(period);
                        }
                    });
            }

            void Stop()
            {
                m_running.store(false);
                if (m_worker.joinable())
                {
                    m_worker.join();
                }
            }

        private:
            std::atomic<bool> m_running{ false };
            std::thread m_worker;
        };

    } // namespace plugin_null
} // namespace camera
