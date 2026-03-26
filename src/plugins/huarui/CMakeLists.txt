add_library(CameraHuarui SHARED
    HuaruiSdkWrapper.h
    HuaruiSdkWrapper.cpp
    HuaruiCamera.h
    HuaruiCamera.cpp
)

target_include_directories(CameraHuarui PRIVATE ${CMAKE_SOURCE_DIR}/SDK/05_Huarui/Include)
target_link_libraries(CameraHuarui PRIVATE CameraCore)
target_link_libraries(CameraHuarui PRIVATE ${CMAKE_SOURCE_DIR}/SDK/05_Huarui/Lib/x64/MVSDKmd.lib)

install(TARGETS CameraHuarui
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
