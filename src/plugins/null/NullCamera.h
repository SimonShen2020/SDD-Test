#pragma once

#include "core/CameraBase.h"

namespace CAM
{

    /**
     * @brief 无硬件依赖的空实现相机。
     *
     * 该实现用于测试、联调和无设备环境下验证 Core 流程。
     */
    class NullCamera : public CameraBase
    {
    public:
        /**
         * @brief 创建一个空实现相机。
         * @param name 相机逻辑名称。
         */
        explicit NullCamera(const std::string& name);

        /**
         * @brief 销毁空实现相机。
         */
        ~NullCamera() override;

        /**
         * @brief 将空实现相机标记为已打开。
         * @return 始终返回 true。
         */
        bool open() override;

        /**
         * @brief 将空实现相机标记为已关闭。
         */
        void close() override;
        /**
         * @brief 开始采集。
         * @return 相机未打开时返回 false，否则返回 true。
         */
        bool startGrabbing() override;

        /**
         * @brief 停止采集。
         */
        void stopGrabbing() override;
    };

} // namespace CAM
