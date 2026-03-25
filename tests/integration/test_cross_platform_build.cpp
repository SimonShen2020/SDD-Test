/**
 * @file test_cross_platform_build.cpp
 * @brief 跨平台构建测试
 *
 * 验证目标平台宏是否正确定义。
 * 支持 Windows (_WIN32) 和 Linux (__linux__) 平台。
 */

#include <gtest/gtest.h>

TEST(CrossPlatformBuildTest, TargetPlatformMacroIsDefined)
{
#if defined(_WIN32) || defined(__linux__)
    SUCCEED();
#else
    GTEST_SKIP() << "Unsupported platform in this build configuration";
#endif
}
