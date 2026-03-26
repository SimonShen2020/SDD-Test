// Module: Core
// Responsibility boundary: Manage vendor camera creators via registry.
// Dependency red line: This module must not include any vendor SDK headers.

#include "camera/core/CameraFactory.hpp"

namespace camera
{
    namespace core
    {

        CameraFactory& CameraFactory::Instance()
        {
            static CameraFactory instance;
            return instance;
        }

        CoreResult CameraFactory::Register(const std::string& vendorType, CameraCreator creator)
        {
            if (vendorType.empty() || !creator)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "invalid factory register input");
            }

            std::lock_guard<std::mutex> lock(m_mutex);
            const auto it = m_creators.find(vendorType);
            if (it != m_creators.end())
            {
                return CoreResult::Failure(CoreErrorCode::kAlreadyExists, "vendor type already registered");
            }

            m_creators.emplace(vendorType, std::move(creator));
            return CoreResult::Success();
        }

        CoreResult CameraFactory::Create(const std::string& vendorType, std::shared_ptr<ICamera>* outCamera) const
        {
            if (outCamera == nullptr)
            {
                return CoreResult::Failure(CoreErrorCode::kInvalidArgument, "outCamera is null");
            }

            std::lock_guard<std::mutex> lock(m_mutex);
            const auto it = m_creators.find(vendorType);
            if (it == m_creators.end())
            {
                return CoreResult::Failure(CoreErrorCode::kNotFound, "vendor type is not registered");
            }

            *outCamera = it->second();
            if (!*outCamera)
            {
                return CoreResult::Failure(CoreErrorCode::kRuntimeError, "creator returned null camera");
            }
            return CoreResult::Success();
        }

    } // namespace core
} // namespace camera
