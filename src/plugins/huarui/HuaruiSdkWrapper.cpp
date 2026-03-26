#include "HuaruiSdkWrapper.h"
#include "core/Log.h"

namespace CAM
{

    void HuaruiSdkWrapper::HandleDeleter::operator()(IMV_HANDLE handle) const
    {
        if (handle)
        {
            IMV_DestroyHandle(handle);
        }
    }


    bool HuaruiSdkWrapper::initialize()
    {
        LOG_INFO("Huarui SDK initialized.");
        return true;
    }


    void HuaruiSdkWrapper::finalize()
    {
        LOG_INFO("Huarui SDK finalized.");
    }


    HuaruiSdkWrapper::HuaruiSdkWrapper() : m_handle(nullptr)
    {
    }


    HuaruiSdkWrapper::~HuaruiSdkWrapper()
    {
        closeDevice();
    }


    bool HuaruiSdkWrapper::openDevice(const std::string& name)
    {
        // This is a simplified implementation. A real implementation would
        // enumerate devices and find the one with the matching user defined name.
        IMV_HANDLE rawHandle = nullptr;
        if (IMV_OK != IMV_CreateHandle(&rawHandle, modeByDeviceUserID, (void*)name.c_str()))
        {
            LOG_ERROR("Failed to create Huarui device handle.");
            return false;
        }

        if (IMV_OK != IMV_Open(rawHandle))
        {
            LOG_ERROR("Failed to open Huarui device.");
            IMV_DestroyHandle(rawHandle);
            return false;
        }

        m_handle.reset(rawHandle);
        LOG_INFO("Huarui device opened.");
        return true;
    }


    void HuaruiSdkWrapper::closeDevice()
    {
        if (m_handle)
        {
            IMV_Close(m_handle.get());
            m_handle.reset();
            LOG_INFO("Huarui device closed.");
        }
    }


    bool HuaruiSdkWrapper::isOpen() const
    {
        return m_handle != nullptr;
    }


    bool HuaruiSdkWrapper::startGrabbing()
    {
        if (IMV_OK != IMV_StartGrabbing(m_handle.get()))
        {
            LOG_ERROR("Failed to start grabbing on Huarui device.");
            return false;
        }
        LOG_INFO("Huarui device started grabbing.");
        return true;
    }


    void HuaruiSdkWrapper::stopGrabbing()
    {
        if (m_handle)
        {
            IMV_StopGrabbing(m_handle.get());
            LOG_INFO("Huarui device stopped grabbing.");
        }
    }

} // namespace CAM
