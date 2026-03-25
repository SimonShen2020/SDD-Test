add_library(CameraCore SHARED
    src/CameraCoreStub.cpp
    src/CameraBase.cpp
    src/CameraFactory.cpp
    src/PluginLoader.cpp
    src/CameraRuntime.cpp
    src/CameraLayout.cpp
    src/CameraConfigParser.cpp
    src/CameraMonitor.cpp
    src/CoreResult.cpp
    src/CameraService.cpp
    src/CameraServiceAlgorithms.cpp
)

target_include_directories(CameraCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(CameraCore
    PUBLIC
        CameraCommon
)

camera_apply_default_compiler_options(CameraCore)
camera_enable_sanitizers(CameraCore)

set_target_properties(CameraCore PROPERTIES
    OUTPUT_NAME "CameraCore"
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)
