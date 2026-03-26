#pragma once

#include <string>

namespace CAM
{

    /**
     * @brief 相机设备的抽象契约。
     *
     * 所有厂商实现都必须遵守该接口，Core 只依赖此抽象，不直接依赖任何 SDK 头文件。
     * 该接口不拥有外部资源；实现类负责管理自身连接、采集流和底层句柄的生命周期。
     *
     * @note 除非实现类另有明确说明，否则该接口的成员函数默认不保证线程安全。
     */
    class ICamera
    {
    public:
        virtual ~ICamera() = default;

        /**
         * @brief 打开相机设备。
         * @return 打开成功返回 true；设备不可用、参数非法或底层 SDK 失败时返回 false。
         * @post 返回 true 时，后续的采集相关接口可以被调用。
         */
        virtual bool open() = 0;

        /**
         * @brief 关闭相机设备。
         * @post 调用后设备进入已关闭状态；重复调用应保持幂等或由实现明确约束。
         */
        virtual void close() = 0;

        /**
         * @brief 判断相机是否处于打开状态。
         * @return 处于可用打开状态返回 true，否则返回 false。
         */
        virtual bool isOpen() const = 0;

        /**
         * @brief 启动图像采集。
         * @return 启动成功返回 true；设备未打开、流已启动或底层 SDK 失败时返回 false。
         * @pre 相机应已打开。
         */
        virtual bool startGrabbing() = 0;

        /**
         * @brief 停止图像采集。
         * @post 返回后不应再继续向上层投递新帧。
         */
        virtual void stopGrabbing() = 0;

        /**
         * @brief 获取相机的逻辑名称。
         * @return 相机名称副本；如果实现无法提供名称，应返回空字符串。
         */
        virtual std::string getName() const = 0;

        /**
         * @brief 获取相机厂商名称。
         * @return 厂商名称副本；如果实现无法提供名称，应返回空字符串。
         */
        virtual std::string getVendorName() const = 0;
    };

} // namespace CAM
