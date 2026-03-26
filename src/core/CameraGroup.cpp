#include "CameraGroup.h"
#include "Log.h"

namespace CAM
{

    CameraGroup::~CameraGroup()
    {
        closeAll();
    }

    void CameraGroup::addCamera(const std::string& name, std::shared_ptr<ICamera> camera)
    {
        if (!camera)
            return;
        m_cameras[name] = camera;
    }

    std::shared_ptr<ICamera> CameraGroup::getCamera(const std::string& name) const
    {
        auto it = m_cameras.find(name);
        if (it != m_cameras.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void CameraGroup::closeAll()
    {
        for (auto& pair : m_cameras)
        {
            if (pair.second && pair.second->isOpen())
            {
                pair.second->close();
            }
        }
    }

} // namespace CAM
