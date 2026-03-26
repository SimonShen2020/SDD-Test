#pragma once

#include "CameraGroup.h"
#include <memory>
#include <string>

namespace CAM
{

    /**
     * @brief 相机服务入口，负责装载配置并组织 CameraGroup。
     *
     * 该类采用单例形式，便于在 Core 层统一协调配置、工厂创建和组管理。
     * 配置解析失败时，初始化返回 false，调用方应直接终止后续依赖流程。
     */
    class CameraService
    {
    public:
        /**
         * @brief 获取服务单例。
         * @return 全局唯一的服务实例引用。
         */
        static CameraService& instance();
        ~CameraService();

        /**
         * @brief 从配置文件初始化相机服务。
         * @param configPath 配置文件路径，必须指向可读取的 JSON 文件。
         * @return 初始化成功返回 true；打开、解析、或相机创建失败时返回 false。
         * @note 初始化过程中若部分相机创建失败，服务会尽量继续处理其余相机。
         */
        bool initialize(const std::string& configPath);

        /**
         * @brief 释放服务持有的相机组资源。
         * @post 调用后组内相机将被停止采集，但对象本身是否继续存活由共享所有权决定。
         */
        void finalize();

        /**
         * @brief 获取当前相机组。
         * @return 当前组的共享所有权句柄；未初始化或已析构时返回空 shared_ptr。
         */
        std::shared_ptr<CameraGroup> getGroup();

    private:
        CameraService();

        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };

} // namespace CAM
