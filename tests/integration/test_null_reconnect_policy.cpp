/**
 * @file test_null_reconnect_policy.cpp
 * @brief 相机重连策略集成测试
 *
 * 验证 CameraMonitor 的重连机制：
 * - 当相机离线时触发重连尝试
 * - 达到最大重连次数后停止尝试
 */

#include <gtest/gtest.h>

#include <atomic>
#include <thread>

#include "camera/core/CameraMonitor.hpp"

TEST(CameraMonitorTest, StopsAtMaxReconnectAttempts)
{
    camera::core::CameraMonitor monitor;
    camera::core::MonitorPolicy policy;
    policy.monitorPeriod = std::chrono::milliseconds(20);
    policy.reconnectInterval = std::chrono::milliseconds(10);
    policy.maxReconnectAttempts = 3;

    std::atomic<bool> online{ false };

    monitor.Start(
        policy,
        [&online]()
        {
            return online.load();
        },
        []()
        {
            return false;
        });

    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    monitor.Stop();

    EXPECT_EQ(monitor.reconnectAttempts(), 3);
}
