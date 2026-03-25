camera_ensure_gtest()

add_executable(camera_core_tests
    unit/test_placeholder.cpp
    unit/test_factory_registration.cpp
    unit/test_camera_layout.cpp
    integration/test_plugin_loading.cpp
    integration/test_3x8_xml_config.cpp
    unit/test_error_semantics.cpp
    integration/test_null_capture_flow.cpp
    integration/test_null_reconnect_policy.cpp
    integration/test_huarui_minimal_flow.cpp
    unit/test_service_contracts.cpp
    integration/test_cross_platform_build.cpp
)

target_compile_definitions(camera_core_tests
    PRIVATE
        CAMERA_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

add_custom_command(TARGET camera_core_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:CameraCore>
        $<TARGET_FILE_DIR:camera_core_tests>
)

target_link_libraries(camera_core_tests
    PRIVATE
        CameraCore
        GTest::gtest_main
)

camera_apply_default_compiler_options(camera_core_tests)
camera_enable_sanitizers(camera_core_tests)

include(GoogleTest)
gtest_discover_tests(camera_core_tests DISCOVERY_MODE PRE_TEST)
