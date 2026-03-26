#pragma once

#include <string>

#include "camera/core/CameraBase.hpp"
#include "camera/plugins/huarui/HuaruiSdkAdapter.hpp"

namespace camera
{
    namespace plugin_huarui
    {

        class HuaruiCamera final : public core::CameraBase
        {
        public:
            HuaruiCamera() = default;
            ~HuaruiCamera() override = default;

            core::CoreResult Capture(core::FrameData* outFrame) override;
            core::CoreResult SetParameter(const std::string& key, const std::string& value) override;
            core::CoreResult GetParameter(const std::string& key, std::string* outValue) const override;

        protected:
            core::CoreResult DoInitialize() override;
            core::CoreResult DoShutdown() override;

        private:
            HuaruiSdkAdapter m_adapter;
            std::string m_exposureMode = "auto";
        };

    } // namespace plugin_huarui
} // namespace camera
