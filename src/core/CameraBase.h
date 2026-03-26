#pragma once

#include "ICamera.h"
#include <string>

namespace CAM
{

    /**
     * @brief 相机公共基类，封装名称、厂商和打开状态。
     *
     * 派生类负责实现底层设备操作，基类仅提供通用状态管理与只读访问接口。
     */
    class CameraBase : public ICamera
    {
    public:
        /**
         * @brief 构造一个带逻辑名称和厂商名称的相机对象。
         * @param name 相机逻辑名称，通常来自配置文件。
         * @param vendorName 厂商名称，用于工厂和日志标识。
         */
        CameraBase(std::string name, std::string vendorName);

        virtual ~CameraBase() = default;

        /**
         * @brief 判断当前对象是否处于已打开状态。
         * @return 已打开返回 true，否则返回 false。
         */
        bool isOpen() const override;

        /**
         * @brief 获取相机逻辑名称。
         * @return 名称副本。
         */
        std::string getName() const override;

        /**
         * @brief 获取厂商名称。
         * @return 厂商名称副本。
         */
        std::string getVendorName() const override;

    protected:
        std::string m_name;
        std::string m_vendorName;
        bool m_isOpen = false;
    };

} // namespace CAM
