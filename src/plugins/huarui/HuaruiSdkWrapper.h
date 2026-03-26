#pragma once

#include "IMV/IMVApi.h"
#include <memory>
#include <string>

namespace CAM
{

    /**
     * @brief 华睿 IMV SDK 资源包装器。
     *
     * 该类把 SDK 初始化、设备句柄和采集控制封装为 RAII 对象，避免裸句柄泄露到上层。
     */
    class HuaruiSdkWrapper
    {
    public:
        /**
         * @brief 构造包装器。
         */
        HuaruiSdkWrapper();

        /**
         * @brief 释放当前持有的设备句柄。
         */
        ~HuaruiSdkWrapper();

        /**
         * @brief 初始化华睿 SDK 全局运行环境。
         * @return 初始化成功返回 true，否则返回 false。
         */
        static bool initialize();

        /**
         * @brief 释放华睿 SDK 全局运行环境。
         */
        static void finalize();

        /**
         * @brief 按逻辑名称打开设备。
         * @param name 设备用户自定义名称。
         * @return 打开成功返回 true，否则返回 false。
         */
        bool openDevice(const std::string& name);

        /**
         * @brief 关闭当前设备并释放句柄。
         */
        void closeDevice();

        /**
         * @brief 判断当前是否存在已打开设备。
         * @return 打开状态返回 true，否则返回 false。
         */
        bool isOpen() const;

        /**
         * @brief 启动设备采集。
         * @return 启动成功返回 true，否则返回 false。
         */
        bool startGrabbing();

        /**
         * @brief 停止设备采集。
         */
        void stopGrabbing();

    private:
        struct HandleDeleter
        {
            void operator()(IMV_HANDLE handle) const;
        };

        std::unique_ptr<std::remove_pointer_t<IMV_HANDLE>, HandleDeleter> m_handle;
    };

} // namespace CAM
