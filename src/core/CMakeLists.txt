add_library(CameraCore SHARED
    ICamera.h
    CameraFactory.h
    CameraFactory.cpp
    CameraGroup.h
    CameraGroup.cpp
    CameraBase.h
    CameraBase.cpp
    CameraService.h
    CameraService.cpp
    Log.h
)

target_include_directories(CameraCore PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..> 
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(CameraCore PRIVATE nlohmann_json)

# Handle symbol visibility
if(WIN32)
    target_compile_definitions(CameraCore PUBLIC "CAM_API_EXPORT")
else()
    # For other platforms, you might need different visibility settings
endif()

install(TARGETS CameraCore
    EXPORT CameraCoreTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT CameraCoreTargets
    FILE CameraCoreTargets.cmake
    NAMESPACE cam::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CameraCore
)

set_target_properties(CameraCore PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

