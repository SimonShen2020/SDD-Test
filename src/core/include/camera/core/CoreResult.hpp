#pragma once

#include <string>
#include <utility>

namespace camera
{
    namespace core
    {

        enum class CoreErrorCode
        {
            kOk = 0,
            kInvalidArgument,
            kNotFound,
            kAlreadyExists,
            kPluginLoadFailed,
            kPluginRegisterFailed,
            kRuntimeError,
            kTimeout,
        };

        struct CoreResult
        {
            CoreErrorCode code = CoreErrorCode::kOk;
            std::string message;
            int vendorCode = 0;

            bool ok() const
            {
                return code == CoreErrorCode::kOk;
            }

            static CoreResult Success()
            {
                return CoreResult{};
            }

            static CoreResult Failure(CoreErrorCode c, std::string msg, int vendor = 0)
            {
                CoreResult result;
                result.code = c;
                result.message = std::move(msg);
                result.vendorCode = vendor;
                return result;
            }
        };

        const char* ToString(CoreErrorCode code);

        void LogIfError(const CoreResult& result, const std::string& context);

    } // namespace core
} // namespace camera
