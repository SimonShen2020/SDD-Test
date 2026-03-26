#pragma once

#include "core/CameraBase.h"
#include "HuaruiSdkWrapper.h"

namespace CAM
{

    /**
     * @brief 华睿 SDK 相机实现。
     *
     * 该类负责将 Core 抽象映射到华睿 IMV SDK，并通过包装器管理底层句柄生命周期。
     */
    class HuaruiCamera : public CameraBase
    {
    public:
        /**
         * @brief 创建一个华睿相机对象。
         * @param name 相机逻辑名称。
         */
        explicit HuaruiCamera(const std::string& name);

        /**
         * @brief 销毁华睿相机对象。
         */
        ~HuaruiCamera() override;

        /**
         * @brief 打开对应的华睿设备。
         * @return 打开成功返回 true，否则返回 false。
         */
        bool open() override;

        /**
         * @brief 关闭对应的华睿设备。
         */
        void close() override;
        /**
         * @brief 启动华睿设备采集。
         * @return 启动成功返回 true，否则返回 false。
         */
        bool startGrabbing() override;

        /**
         * @brief 停止华睿设备采集。
         */
        void stopGrabbing() override;

    private:
        HuaruiSdkWrapper m_sdkWrapper;
    };

} // namespace CAM
