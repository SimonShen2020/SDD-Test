#pragma once

#include "ICamera.h"
#include <map>
#include <memory>
#include <string>

namespace CAM
{

    /**
     * @brief 按逻辑名称管理一组相机实例。
     *
     * 该类只负责容器与生命周期托管，不负责创建设备，也不负责枚举硬件。
     * 对外访问通过共享所有权返回相机，避免暴露裸指针和悬空引用风险。
     */
    class CameraGroup
    {
    public:
        CameraGroup() = default;
        ~CameraGroup();

        /**
         * @brief 按名称加入或覆盖一个相机实例。
         * @param name 相机的唯一逻辑名称，不能为空。
         * @param camera 要托管的相机实例；空指针会被忽略。
         * @post 名称已存在时会覆盖旧实例，由共享所有权决定旧对象何时释放。
         */
        void addCamera(const std::string& name, std::shared_ptr<ICamera> camera);

        /**
         * @brief 根据名称查找相机。
         * @param name 相机逻辑名称。
         * @return 找到时返回共享所有权句柄；未找到时返回空 shared_ptr。
         */
        std::shared_ptr<ICamera> getCamera(const std::string& name) const;

        /**
         * @brief 停止当前组内所有仍处于打开状态的相机。
         * @note 该操作只负责停止采集，不会移除容器中的相机对象。
         */
        void closeAll();

    private:
        std::map<std::string, std::shared_ptr<ICamera>> m_cameras;
    };

} // namespace CAM
