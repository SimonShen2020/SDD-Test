#include "CameraBase.h"

namespace CAM
{

    CameraBase::CameraBase(std::string name, std::string vendorName) : m_name(std::move(name)), m_vendorName(std::move(vendorName)), m_isOpen(false)
    {
    }


    bool CameraBase::isOpen() const
    {
        return m_isOpen;
    }


    std::string CameraBase::getName() const
    {
        return m_name;
    }


    std::string CameraBase::getVendorName() const
    {
        return m_vendorName;
    }

} // namespace CAM
