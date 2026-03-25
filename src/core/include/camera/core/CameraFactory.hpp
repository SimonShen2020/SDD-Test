#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "camera/core/CoreResult.hpp"
#include "camera/core/ICamera.hpp"

namespace camera
{
    namespace core
    {

        using CameraCreator = std::function<std::shared_ptr<ICamera>()>;

        class CameraFactory
        {
        public:
            static CameraFactory& Instance();

            CoreResult Register(const std::string& vendorType, CameraCreator creator);
            CoreResult Create(const std::string& vendorType, std::shared_ptr<ICamera>* outCamera) const;

        private:
            CameraFactory() = default;

            mutable std::mutex mutex_;
            std::unordered_map<std::string, CameraCreator> creators_;
        };

    } // namespace core
} // namespace camera
