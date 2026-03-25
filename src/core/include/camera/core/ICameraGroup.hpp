#pragma once

#include <memory>
#include <string>
#include <vector>

#include "camera/core/ICamera.hpp"

namespace camera
{
    namespace core
    {

        /**
         * @brief 相机组接口抽象基类
         *
         * 管理一组相机的集合，提供按用户ID访问相机和列出所有相机ID的功能。
         * 用于表示物理相机阵列或逻辑相机分组。
         */
        class ICameraGroup
        {
          public:
            virtual ~ICameraGroup() = default;

            /**
             * @brief 获取组内相机数量
             * @return 相机数量
             */
            virtual std::size_t Size() const = 0;

            /**
             * @brief 根据用户ID获取相机实例
             * @param userId 用户定义的唯一标识符
             * @return 相机实例指针，如果不存在返回 nullptr
             */
            virtual std::shared_ptr<ICamera> GetByUserId(const std::string& userId) const = 0;

            /**
             * @brief 列出组内所有相机的用户ID
             * @return 用户ID列表
             */
            virtual std::vector<std::string> ListUserIds() const = 0;
        };

    } // namespace core
} // namespace camera
