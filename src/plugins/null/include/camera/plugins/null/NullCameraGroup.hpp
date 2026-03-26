#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "camera/core/ICameraGroup.hpp"

namespace camera
{
    namespace plugin_null
    {

        class NullCameraGroup final : public core::ICameraGroup
        {
        public:
            NullCameraGroup();
            ~NullCameraGroup() override = default;

            std::size_t Size() const override;
            std::shared_ptr<core::ICamera> GetByUserId(const std::string& userId) const override;
            std::vector<std::string> ListUserIds() const override;

        private:
            std::unordered_map<std::string, std::shared_ptr<core::ICamera>> m_cameras;
        };

    } // namespace plugin_null
} // namespace camera
