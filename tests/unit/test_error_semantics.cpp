/**
 * @file test_error_semantics.cpp
 * @brief CoreResult 错误语义测试
 *
 * 验证 CoreResult 结构体的错误信息传递功能：
 * - 错误码、错误消息、厂商错误码的正确传递
 * - Failure 工厂方法创建的失败结果包含完整信息
 */

#include <gtest/gtest.h>

#include "camera/core/CoreResult.hpp"

TEST(CoreResultTest, ContainsVendorCodeAndMessage)
{
    auto failure = camera::core::CoreResult::Failure(camera::core::CoreErrorCode::kPluginRegisterFailed, "register failed", 42);

    EXPECT_FALSE(failure.ok());
    EXPECT_EQ(failure.vendorCode, 42);
    EXPECT_EQ(failure.message, "register failed");
}
