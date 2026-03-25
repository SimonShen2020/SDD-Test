/**
 * @file test_plugin_loading.cpp
 * @brief 插件加载集成测试
 *
 * 验证 PluginLoader 的插件加载和注册功能：
 * - 处理不存在的插件路径
 * - 处理缺失的注册符号
 */

#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "camera/core/CoreResult.hpp"
#include "camera/core/PluginLoader.hpp"

TEST(PluginLoaderTest, FailsWhenPluginPathMissing)
{
    camera::core::PluginLoader loader;
    std::size_t loadedCount = 0;
    auto result = loader.LoadAndRegister({ "non_existing_plugin.dll" }, "RegisterCameraPlugin", &loadedCount);

    EXPECT_FALSE(result.ok());
    EXPECT_EQ(loadedCount, 0U);
    EXPECT_EQ(result.code, camera::core::CoreErrorCode::kPluginLoadFailed);
}

TEST(PluginLoaderTest, FailsWhenRegisterSymbolMissing)
{
    std::filesystem::path pluginPath = std::filesystem::path(CAMERA_SOURCE_DIR) / "build" / "src" / "plugins" / "null" / "Debug" / "CameraNull.dll";
    if (!std::filesystem::exists(pluginPath))
    {
        GTEST_SKIP() << "CAMERA_NULL_PLUGIN_PATH is not set";
    }

    camera::core::PluginLoader loader;
    std::size_t loadedCount = 0;
    auto result = loader.LoadAndRegister({ pluginPath.string() }, "MissingRegisterSymbol", &loadedCount);

    EXPECT_FALSE(result.ok());
    EXPECT_EQ(loadedCount, 0U);
    EXPECT_EQ(result.code, camera::core::CoreErrorCode::kPluginRegisterFailed);
}
