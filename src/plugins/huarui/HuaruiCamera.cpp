#include "HuaruiCamera.h"
#include "core/CameraFactory.h"
#include "core/Log.h"

namespace CAM
{

    REGISTER_CAMERA("Huarui", HuaruiCamera);

    HuaruiCamera::HuaruiCamera(const std::string& name) : CameraBase(name, "Huarui")
    {
        LOG_INFO("HuaruiCamera created with name: " + name);
        HuaruiSdkWrapper::initialize();
    }


    HuaruiCamera::~HuaruiCamera()
    {
        LOG_INFO("HuaruiCamera destroyed.");
        HuaruiSdkWrapper::finalize();
    }


    bool HuaruiCamera::open()
    {
        // In a real scenario, the name would be passed in here
        // and used to open the correct device.
        m_isOpen = m_sdkWrapper.openDevice(m_name);
        return m_isOpen;
    }


    void HuaruiCamera::close()
    {
        m_sdkWrapper.closeDevice();
        m_isOpen = false;
    }


    bool HuaruiCamera::startGrabbing()
    {
        if (!m_isOpen)
        {
            LOG_ERROR("Huarui camera is not open.");
            return false;
        }
        return m_sdkWrapper.startGrabbing();
    }


    void HuaruiCamera::stopGrabbing()
    {
        m_sdkWrapper.stopGrabbing();
    }

} // namespace CAM
