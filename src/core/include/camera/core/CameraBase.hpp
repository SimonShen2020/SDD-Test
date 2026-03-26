#pragma once

#include <atomic>

#include "camera/core/ICamera.hpp"

namespace camera
{
    namespace core
    {

        class CameraBase : public ICamera
        {
        public:
            CameraBase() = default;
            ~CameraBase() override = default;

            CoreResult Initialize() override;
            CoreResult Shutdown() override;

        protected:
            virtual CoreResult DoInitialize() = 0;
            virtual CoreResult DoShutdown() = 0;

        private:
            std::atomic<bool> m_initialized{ false };
        };

    } // namespace core
} // namespace camera
