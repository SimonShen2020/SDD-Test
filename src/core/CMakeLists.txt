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

add_library(CameraSDD::CameraCore ALIAS CameraCore)

target_include_directories(CameraCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(CameraCore
    PUBLIC
        CameraSDD::CameraCommon
)

if(CMAKE_DL_LIBS)
    target_link_libraries(CameraCore PRIVATE ${CMAKE_DL_LIBS})
endif()

camera_apply_default_compiler_options(CameraCore)
camera_enable_sanitizers(CameraCore)

set_target_properties(CameraCore PROPERTIES
    OUTPUT_NAME "CameraCore"
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

install(TARGETS CameraCore
    EXPORT CameraSDDTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
