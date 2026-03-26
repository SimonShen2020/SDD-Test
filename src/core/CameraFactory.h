#pragma once

#include "ICamera.h"
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace CAM
{

    /**
     * @brief 相机创建器函数签名。
     * @param name 逻辑相机名称，由上层配置传入。
     * @return 新创建的相机实例所有权句柄。
     */
    using CameraCreator = std::function<std::unique_ptr<ICamera>(const std::string& name)>;

    /**
     * @brief 按厂商名称注册和创建相机实例的工厂。
     *
     * 工厂通过自注册机制聚合插件实现，Core 不直接依赖任何厂商 SDK。
     */
    class CameraFactory
    {
    public:
        /**
         * @brief 获取工厂单例。
         * @return 全局唯一的工厂实例引用。
         */
        static CameraFactory& instance();

        /**
         * @brief 注册某厂商对应的相机创建器。
         * @param vendorName 厂商名称，作为查找键使用。
         * @param creator 创建器回调；若同名已存在则覆盖原注册项。
         */
        void registerCreator(const std::string& vendorName, CameraCreator creator);

        /**
         * @brief 按厂商和相机名称创建实例。
         * @param vendorName 厂商名称。
         * @param cameraName 逻辑相机名称。
         * @return 创建成功返回相机实例；厂商未注册或创建失败时返回空 unique_ptr。
         */
        std::unique_ptr<ICamera> createCamera(const std::string& vendorName, const std::string& cameraName);

    private:
        CameraFactory() = default;
        std::map<std::string, CameraCreator> m_creators;
    };

#define REGISTER_CAMERA(VendorName, ClassName)                    \
    namespace                                                     \
    {                                                             \
        const bool registered = []()                              \
        {                                                         \
            CameraFactory::instance().registerCreator(VendorName, \
                [](const std::string& name)                       \
                {                                                 \
                    return std::make_unique<ClassName>(name);     \
                });                                               \
            return true;                                          \
        }();                                                      \
    }

} // namespace CAM
