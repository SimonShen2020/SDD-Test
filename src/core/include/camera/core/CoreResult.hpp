#pragma once

#include <string>

namespace camera
{
    namespace core
    {

        /**
         * @brief 核心错误码枚举
         *
         * 定义相机核心模块可能返回的所有错误类型。
         * 错误码按功能分类：参数错误、资源错误、运行时错误等。
         */
        enum class CoreErrorCode
        {
            kOk = 0,               ///< 操作成功
            kInvalidArgument,      ///< 无效参数
            kNotFound,             ///< 资源未找到
            kAlreadyExists,        ///< 资源已存在
            kPluginLoadFailed,     ///< 插件加载失败
            kPluginRegisterFailed, ///< 插件注册失败
            kRuntimeError,         ///< 运行时错误
            kTimeout,              ///< 操作超时
        };

        /**
         * @brief 操作结果结构体
         *
         * 封装操作结果的状态信息，包括错误码、错误消息和厂商特定错误码。
         * 使用工厂方法 Success() 和 Failure() 创建结果实例。
         */
        struct CoreResult
        {
            CoreErrorCode code = CoreErrorCode::kOk;  ///< 错误码
            std::string message;                      ///< 错误描述消息
            int vendorCode = 0;                       ///< 厂商特定错误码（可选）

            /**
             * @brief 检查操作是否成功
             * @return true 如果 code == kOk
             */
            bool ok() const
            {
                return code == CoreErrorCode::kOk;
            }

            /**
             * @brief 创建成功结果
             * @return 默认构造的 CoreResult（code 为 kOk）
             */
            static CoreResult Success()
            {
                return CoreResult{};
            }

            /**
             * @brief 创建失败结果
             * @param c 错误码
             * @param msg 错误描述消息
             * @param vendor 厂商特定错误码（默认为 0）
             * @return 包含错误信息的 CoreResult
             */
            static CoreResult Failure(CoreErrorCode c, std::string msg, int vendor = 0)
            {
                CoreResult result;
                result.code = c;
                result.message = std::move(msg);
                result.vendorCode = vendor;
                return result;
            }
        };

        /**
         * @brief 将错误码转换为字符串表示
         * @param code 错误码
         * @return 错误码的字符串名称
         */
        const char* ToString(CoreErrorCode code);

        /**
         * @brief 如果操作失败则记录错误日志
         * @param result 操作结果
         * @param context 上下文描述信息
         */
        void LogIfError(const CoreResult& result, const std::string& context);

    } // namespace core
} // namespace camera
