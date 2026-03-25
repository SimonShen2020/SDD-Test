/**
 * @file test_huarui_minimal_flow.cpp
 * @brief Huarui 插件最小流程集成测试
 *
 * 验证 Huarui 相机插件的加载和注册功能。
 * 如果插件二进制文件不存在，则跳过测试。
 */

#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "camera/core/PluginLoader.hpp"

TEST(HuaruiPluginTest, CanLoadPluginAndFindRegisterEntry)
{
    std::filesystem::path pluginPath = std::filesystem::path(CAMERA_SOURCE_DIR) / "build" / "src" / "plugins" / "huarui" / "Debug" / "CameraHuarui.dll";

    if (!std::filesystem::exists(pluginPath))
    {
        GTEST_SKIP() << "Huarui plugin binary is not present";
    }

    camera::core::PluginLoader loader;
    std::size_t loadedCount = 0;
    auto result = loader.LoadAndRegister({ pluginPath.string() }, "RegisterCameraPlugin", &loadedCount);

    EXPECT_TRUE(result.ok());
    EXPECT_EQ(loadedCount, 1U);
}
