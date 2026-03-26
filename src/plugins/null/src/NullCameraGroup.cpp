// Module: Plugin/Null
// Responsibility boundary: Provide a virtual camera group for topology and tests.
// Dependency red line: Plugin may depend on Core abstractions but never vice versa.

#include "camera/plugins/null/NullCameraGroup.hpp"

#include "camera/plugins/null/NullCamera.hpp"

namespace camera
{
    namespace plugin_null
    {

        NullCameraGroup::NullCameraGroup()
        {
            m_cameras.emplace("DHAA0", std::make_shared<NullCamera>());
            m_cameras.emplace("DHAA1", std::make_shared<NullCamera>());
        }

        std::size_t NullCameraGroup::Size() const
        {
            return m_cameras.size();
        }

        std::shared_ptr<core::ICamera> NullCameraGroup::GetByUserId(const std::string& userId) const
        {
            const auto it = m_cameras.find(userId);
            if (it == m_cameras.end())
            {
                return {};
            }
            return it->second;
        }

        std::vector<std::string> NullCameraGroup::ListUserIds() const
        {
            std::vector<std::string> ids;
            ids.reserve(m_cameras.size());
            for (const auto& item : m_cameras)
            {
                ids.push_back(item.first);
            }
            return ids;
        }

    } // namespace plugin_null
} // namespace camera
