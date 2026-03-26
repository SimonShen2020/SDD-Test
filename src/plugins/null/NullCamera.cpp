#include "NullCamera.h"
#include "core/CameraFactory.h"
#include "core/Log.h"

namespace CAM
{

    REGISTER_CAMERA("Null", NullCamera);

    NullCamera::NullCamera(const std::string& name) : CameraBase(name, "Null")
    {
        LOG_INFO("NullCamera created with name: " + name);
    }


    NullCamera::~NullCamera()
    {
        LOG_INFO("NullCamera destroyed.");
    }


    bool NullCamera::open()
    {
        LOG_INFO("NullCamera::open() called.");
        m_isOpen = true;
        return true;
    }


    void NullCamera::close()
    {
        LOG_INFO("NullCamera::close() called.");
        m_isOpen = false;
    }


    bool NullCamera::startGrabbing()
    {
        LOG_INFO("NullCamera::startGrabbing() called.");
        if (!m_isOpen)
        {
            LOG_ERROR("Camera is not open.");
            return false;
        }
        return true;
    }


    void NullCamera::stopGrabbing()
    {
        LOG_INFO("NullCamera::stopGrabbing() called.");
    }

} // namespace CAM
