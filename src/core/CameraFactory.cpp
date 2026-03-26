#include "CameraFactory.h"

namespace CAM
{

    CameraFactory& CameraFactory::instance()
    {
        static CameraFactory factory;
        return factory;
    }

    void CameraFactory::registerCreator(const std::string& vendorName, CameraCreator creator)
    {
        m_creators[vendorName] = creator;
    }

    std::unique_ptr<ICamera> CameraFactory::createCamera(const std::string& vendorName, const std::string& cameraName)
    {
        auto it = m_creators.find(vendorName);
        if (it != m_creators.end())
        {
            return it->second(cameraName);
        }
        return nullptr;
    }

} // namespace CAM
