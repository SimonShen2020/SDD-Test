cmake_minimum_required(VERSION 3.16)

project(CameraSDD VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CompilerOptions)
include(Dependencies)
include(Sanitizers)

option(CAMERA_BUILD_TESTS "Build camera tests" ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(src/common)
add_subdirectory(src/core)
add_subdirectory(src/plugins/null)
add_subdirectory(src/plugins/huarui)

if(CAMERA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

set(camera_sdd_cmake_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/CameraSDD")

install(EXPORT CameraSDDTargets
    FILE CameraSDDTargets.cmake
    NAMESPACE CameraSDD::
    DESTINATION ${camera_sdd_cmake_install_dir}
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CameraSDDConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/CameraSDDConfig.cmake
    INSTALL_DESTINATION ${camera_sdd_cmake_install_dir}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/CameraSDDConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/CameraSDDConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/CameraSDDConfigVersion.cmake
    DESTINATION ${camera_sdd_cmake_install_dir}
)
