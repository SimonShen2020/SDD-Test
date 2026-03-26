add_library(CameraNull SHARED
    NullCamera.h
    NullCamera.cpp
)

target_link_libraries(CameraNull PRIVATE CameraCore)

install(TARGETS CameraNull
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
set_target_properties(CameraNull PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
