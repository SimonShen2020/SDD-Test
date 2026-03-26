add_library(CameraHuarui SHARED
    src/CameraHuaruiStub.cpp
    src/HuaruiSdkAdapter.cpp
    src/HuaruiCamera.cpp
    src/HuaruiPluginEntry.cpp
)

add_library(CameraSDD::CameraHuarui ALIAS CameraHuarui)

target_include_directories(CameraHuarui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(CameraHuarui
    PRIVATE
        CameraSDD::CameraCore
)

camera_apply_default_compiler_options(CameraHuarui)
camera_enable_sanitizers(CameraHuarui)

set_target_properties(CameraHuarui PROPERTIES
    OUTPUT_NAME "CameraHuarui"
)

install(TARGETS CameraHuarui
    EXPORT CameraSDDTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
