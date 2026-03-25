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
                running_.store(true);
                worker_ = std::thread(
                    [this, callback, period]()
                    {
                        while (running_.load())
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
                running_.store(false);
                if (worker_.joinable())
                {
                    worker_.join();
                }
            }

        private:
            std::atomic<bool> running_{ false };
            std::thread worker_;
        };

    } // namespace plugin_null
} // namespace camera
