#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "camera/core/CoreResult.hpp"

namespace camera
{
    namespace core
    {

        /**
         * @brief 帧数据结构体
         *
         * 存储图像帧的二进制数据及其元信息，包括尺寸、像素格式和时间戳。
         */
        struct FrameData
        {
            std::vector<std::uint8_t> bytes;  ///< 图像二进制数据
            std::uint32_t width = 0;          ///< 图像宽度（像素）
            std::uint32_t height = 0;         ///< 图像高度（像素）
            std::string pixelFormat;          ///< 像素格式（如 "GRAY8", "RGB24"）
            std::uint64_t timestampNs = 0;    ///< 时间戳（纳秒）
        };

        /**
         * @brief 相机接口抽象基类
         *
         * 定义所有相机实现必须提供的基本操作，包括生命周期管理、图像捕获和参数控制。
         * 所有具体相机实现（如 NullCamera、HuaruiCamera）都必须继承此接口。
         */
        class ICamera
        {
          public:
            virtual ~ICamera() = default;

            /**
             * @brief 初始化相机资源
             * @return CoreResult 操作结果，成功时返回 ok() == true
             */
            virtual CoreResult Initialize() = 0;

            /**
             * @brief 关闭相机并释放资源
             * @return CoreResult 操作结果
             */
            virtual CoreResult Shutdown() = 0;

            /**
             * @brief 捕获一帧图像
             * @param outFrame 输出帧数据指针，不能为 nullptr
             * @return CoreResult 操作结果，成功时 outFrame 包含有效图像数据
             */
            virtual CoreResult Capture(FrameData* outFrame) = 0;

            /**
             * @brief 设置相机参数
             * @param key 参数名称
             * @param value 参数值
             * @return CoreResult 操作结果
             */
            virtual CoreResult SetParameter(const std::string& key, const std::string& value) = 0;

            /**
             * @brief 获取相机参数
             * @param key 参数名称
             * @param outValue 输出参数值指针，不能为 nullptr
             * @return CoreResult 操作结果，成功时 outValue 包含参数值
             */
            virtual CoreResult GetParameter(const std::string& key, std::string* outValue) const = 0;
        };

    } // namespace core
} // namespace camera
