// Module: Core
// Responsibility boundary: Load plugin binaries and trigger registration entry.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/core/PluginLoader.hpp"

#include <string>

#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace camera
{
    namespace core
    {

        CoreResult PluginLoader::LoadAndRegister(const std::vector<std::string>& pluginPaths, const std::string& registerSymbol, std::size_t* loadedCount)
        {
            if (loadedCount == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "loadedCount is null");
            }

            *loadedCount = 0;
            for (const std::string& path : pluginPaths)
            {
                CoreResult single = LoadSingle(path, registerSymbol, loadedCount);
                if (!single.ok())
                {
                    return single;
                }
            }
            return CoreResult::Success();
        }

        CoreResult PluginLoader::LoadSingle(const std::string& pluginPath, const std::string& registerSymbol, std::size_t* loadedCount)
        {
#if defined(_WIN32)
            HMODULE module = LoadLibraryA(pluginPath.c_str());
            if (module == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kPluginLoadFailed, "LoadLibrary failed: " + pluginPath);
            }

            auto registerFn = reinterpret_cast<PluginRegisterFn>(GetProcAddress(module, registerSymbol.c_str()));
            if (registerFn == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kPluginRegisterFailed, "GetProcAddress failed: " + registerSymbol);
            }
#else
            void* module = dlopen(pluginPath.c_str(), RTLD_NOW);
            if (module == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kPluginLoadFailed, "dlopen failed: " + pluginPath);
            }

            auto registerFn = reinterpret_cast<PluginRegisterFn>(dlsym(module, registerSymbol.c_str()));
            if (registerFn == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kPluginRegisterFailed, "dlsym failed: " + registerSymbol);
            }
#endif

            const int registerCode = registerFn();
            if (registerCode != 0)
            {
                return CoreResult::Failure(CoreErrorCode::kPluginRegisterFailed, "plugin register returned non-zero", registerCode);
            }

            ++(*loadedCount);
            return CoreResult::Success();
        }

    } // namespace core
} // namespace camera
