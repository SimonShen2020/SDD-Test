#pragma once

namespace camera
{
    namespace core
    {

        constexpr const char* kDefaultPluginRegisterSymbol = "RegisterCameraPlugin";
        using PluginRegisterFunction = int (*)();

    } // namespace core
} // namespace camera
