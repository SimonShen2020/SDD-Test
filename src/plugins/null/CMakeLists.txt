add_library(CameraNull SHARED
    src/CameraNullStub.cpp
    src/NullPluginEntry.cpp
    src/NullCamera.cpp
    src/NullCameraGroup.cpp
    src/NullStreamSimulator.cpp
)

target_include_directories(CameraNull
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(CameraNull
    PRIVATE
        CameraCore
)

camera_apply_default_compiler_options(CameraNull)
camera_enable_sanitizers(CameraNull)

set_target_properties(CameraNull PROPERTIES
    OUTPUT_NAME "CameraNull"
)
