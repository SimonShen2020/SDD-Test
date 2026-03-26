add_library(CameraCommon STATIC
    src/Log.cpp
    src/StringUtils.cpp
    src/FileUtils.cpp
)

add_library(CameraSDD::CameraCommon ALIAS CameraCommon)

target_include_directories(CameraCommon
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

camera_apply_default_compiler_options(CameraCommon)
camera_enable_sanitizers(CameraCommon)

install(TARGETS CameraCommon
    EXPORT CameraSDDTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
