/**
 * @file test_3x8_xml_config.cpp
 * @brief 3x8 XML 配置解析集成测试
 *
 * 验证 CameraConfigParser 对 3x8 相机布局配置的解析功能：
 * - 正确解析厂商类型、布局尺寸和相机列表
 * - 构建正确的相机拓扑映射
 * - 拒绝缺失字段或无效配置
 */

#include <gtest/gtest.h>

#include <filesystem>

#include "camera/core/CameraConfigParser.hpp"

TEST(CameraConfigParserTest, Parses3x8ConfigAndBuildsTopology)
{
    const std::filesystem::path configPath = std::filesystem::path(CAMERA_SOURCE_DIR) / "doc" / "camera-config-3x8.xml";

    camera::core::CameraConfig config;
    camera::core::CameraLayout layout;
    camera::core::CameraConfigParser parser;

    auto result = parser.ParseFromFile(configPath.string(), &config, &layout);
    ASSERT_TRUE(result.ok()) << result.message;

    EXPECT_EQ(config.vendorType, "null");
    EXPECT_EQ(config.rows, 3);
    EXPECT_EQ(config.cols, 8);
    EXPECT_EQ(config.cameras.size(), 24U);
    EXPECT_EQ(layout.cameraCount(), 24U);

    int row = -1;
    int col = -1;
    auto mapResult = layout.GetPosition("DHAC7", &row, &col);
    ASSERT_TRUE(mapResult.ok());
    EXPECT_EQ(row, 2);
    EXPECT_EQ(col, 7);
}

TEST(CameraConfigParserTest, RejectsMissingOrInvalidFields)
{
    camera::core::CameraConfig config;
    camera::core::CameraLayout layout;
    camera::core::CameraConfigParser parser;

    const std::string missingVendor = "<cameraConfig><layout rows=\"3\" cols=\"8\"/>"
                                      "<cameras><camera userId=\"DHAA0\" deviceId=\"x\" row=\"0\" col=\"0\"/></cameras></cameraConfig>";

    auto missResult = parser.ParseFromString(missingVendor, &config, &layout);
    EXPECT_FALSE(missResult.ok());

    const std::string invalidLayout = "<cameraConfig><vendorType>null</vendorType><layout rows=\"11\" cols=\"1\"/>"
                                      "<cameras><camera userId=\"DHAA0\" deviceId=\"x\" row=\"0\" col=\"0\"/></cameras></cameraConfig>";

    auto layoutResult = parser.ParseFromString(invalidLayout, &config, &layout);
    EXPECT_FALSE(layoutResult.ok());

    const std::string duplicateUserId = "<cameraConfig><vendorType>null</vendorType><layout rows=\"2\" cols=\"2\"/>"
                                        "<cameras>"
                                        "<camera userId=\"DHAA0\" deviceId=\"x\" row=\"0\" col=\"0\"/>"
                                        "<camera userId=\"DHAA0\" deviceId=\"y\" row=\"0\" col=\"1\"/>"
                                        "</cameras></cameraConfig>";

    auto dupResult = parser.ParseFromString(duplicateUserId, &config, &layout);
    EXPECT_FALSE(dupResult.ok());
}
