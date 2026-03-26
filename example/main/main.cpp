#include "core/CameraService.h"
#include "core/Log.h"
#include <iostream>

int main() {
    if (!CAM::CameraService::instance().initialize("config.json")) {
        LOG_ERROR("Failed to initialize CameraService.");
        return -1;
    }

    auto group = CAM::CameraService::instance().getGroup();
    if (!group) {
        LOG_ERROR("CameraGroup is null.");
        return -1;
    }

    auto* camA = group->getCamera("cam00");
    if (camA) {
        camA->open();
        camA->startGrabbing();
        camA->stopGrabbing();
        camA->close();
    }

    auto* camB = group->getCamera("cam01");
    if (camB) {
        camB->open();
        camB->startGrabbing();
        camB->stopGrabbing();
        camB->close();
    }

    CAM::CameraService::instance().finalize();

    return 0;
}
