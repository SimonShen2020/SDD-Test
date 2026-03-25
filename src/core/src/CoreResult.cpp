// Module: Core
// Responsibility boundary: Provide error-code text and unified error logging helper.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/core/CoreResult.hpp"

#include "camera/common/Log.hpp"

namespace camera
{
    namespace core
    {

        const char* ToString(CoreErrorCode code)
        {
            switch (code)
            {
            case CoreErrorCode::kOk:
                return "kOk";
            case CoreErrorCode::kInvalidArgument:
                return "kInvalidArgument";
            case CoreErrorCode::kNotFound:
                return "kNotFound";
            case CoreErrorCode::kAlreadyExists:
                return "kAlreadyExists";
            case CoreErrorCode::kPluginLoadFailed:
                return "kPluginLoadFailed";
            case CoreErrorCode::kPluginRegisterFailed:
                return "kPluginRegisterFailed";
            case CoreErrorCode::kRuntimeError:
                return "kRuntimeError";
            case CoreErrorCode::kTimeout:
                return "kTimeout";
            default:
                return "kUnknown";
            }
        }

        void LogIfError(const CoreResult& result, const std::string& context)
        {
            if (result.ok())
            {
                return;
            }

            camera::common::WriteLogLine(
                "ERROR", context + " code=" + ToString(result.code) + " vendorCode=" + std::to_string(result.vendorCode) + " message=" + result.message);
        }

    } // namespace core
} // namespace camera
