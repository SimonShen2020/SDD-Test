add_library(CameraNull SHARED
    src/CameraNullStub.cpp
    src/NullPluginEntry.cpp
    src/NullCamera.cpp
    src/NullCameraGroup.cpp
    src/NullStreamSimulator.cpp
)

add_library(CameraSDD::CameraNull ALIAS CameraNull)

target_include_directories(CameraNull
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(CameraNull
    PRIVATE
        CameraSDD::CameraCore
)

camera_apply_default_compiler_options(CameraNull)
camera_enable_sanitizers(CameraNull)

set_target_properties(CameraNull PROPERTIES
    OUTPUT_NAME "CameraNull"
)

install(TARGETS CameraNull
    EXPORT CameraSDDTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
