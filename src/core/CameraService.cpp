#include "CameraService.h"
#include "CameraFactory.h"
#include "Log.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

namespace CAM
{

    class CameraService::Impl
    {
    public:
        std::shared_ptr<CameraGroup> m_group;
        std::string m_vendor;

        Impl() : m_group(std::make_shared<CameraGroup>())
        {
        }
    };

    CameraService& CameraService::instance()
    {
        static CameraService service;
        return service;
    }

    CameraService::CameraService() : m_pImpl(std::make_unique<Impl>())
    {
    }

    CameraService::~CameraService()
    {
        finalize();
    }

    bool CameraService::initialize(const std::string& configPath)
    {
        std::ifstream f(configPath);
        if (!f.is_open())
        {
            LOG_ERROR("Failed to open config file: " + configPath);
            return false;
        }

        json data;
        try
        {
            data = json::parse(f);
        }
        catch (json::parse_error& e)
        {
            LOG_ERROR("Failed to parse config file: " + std::string(e.what()));
            return false;
        }

        m_pImpl->m_vendor = data["vendor"];

        for (const auto& cam_info : data["cameras"])
        {
            std::string name = cam_info["name"];

            auto camera = CameraFactory::instance().createCamera(m_pImpl->m_vendor, name);
            if (!camera)
            {
                LOG_ERROR("Failed to create camera for vendor: " + m_pImpl->m_vendor);
                continue;
            }

            // Ideally the system opens the camera by name inside its own initialization if needed.
            // Or we pass the name to the camera via an interface method.
            // For simplicity, we just add it to the group, and assume the plugin manages its own identification
            // or receives it during open(name).

            m_pImpl->m_group->addCamera(name, camera);
        }

        LOG_INFO("CameraService initialized successfully.");
        return true;
    }

    void CameraService::finalize()
    {
        if (m_pImpl && m_pImpl->m_group)
        {
            m_pImpl->m_group->closeAll();
        }
        LOG_INFO("CameraService finalized.");
    }

    std::shared_ptr<CameraGroup> CameraService::getGroup()
    {
        return m_pImpl ? m_pImpl->m_group : nullptr;
    }

} // namespace CAM
