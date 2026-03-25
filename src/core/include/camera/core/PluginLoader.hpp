#pragma once

#include <functional>
#include <string>
#include <vector>

#include "camera/core/CoreResult.hpp"

namespace camera
{
    namespace core
    {

        using PluginRegisterFn = int (*)();

        class PluginLoader
        {
        public:
            CoreResult LoadAndRegister(const std::vector<std::string>& pluginPaths, const std::string& registerSymbol, std::size_t* loadedCount);

        private:
            CoreResult LoadSingle(const std::string& pluginPath, const std::string& registerSymbol, std::size_t* loadedCount);
        };

    } // namespace core
} // namespace camera
