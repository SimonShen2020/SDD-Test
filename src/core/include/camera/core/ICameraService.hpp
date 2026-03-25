#pragma once

#include <string>

#include "camera/core/CoreResult.hpp"

namespace camera
{
    namespace core
    {

        /**
         * @brief 相机服务接口抽象基类
         *
         * 提供相机系统的顶层服务接口，包括初始化、关闭和单次捕获功能。
         * 这是客户端代码与相机系统交互的主要入口点。
         */
        class ICameraService
        {
          public:
            virtual ~ICameraService() = default;

            /**
             * @brief 初始化相机服务
             * @param configPath 配置文件路径
             * @return CoreResult 操作结果
             */
            virtual CoreResult Initialize(const std::string& configPath) = 0;

            /**
             * @brief 关闭相机服务
             * @return CoreResult 操作结果
             */
            virtual CoreResult Shutdown() = 0;

            /**
             * @brief 单次捕获指定相机的图像
             * @param userId 相机用户ID
             * @return CoreResult 操作结果
             */
            virtual CoreResult CaptureOnce(const std::string& userId) = 0;
        };

    } // namespace core
} // namespace camera
