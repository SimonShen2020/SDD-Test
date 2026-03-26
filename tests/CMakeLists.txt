add_executable(cameratest
    factory_test.cpp
    service_layout_test.cpp
    service_call_test.cpp
    camera_group_test.cpp
    service_failure_test.cpp
    e2e_integration_test.cpp
)

target_link_libraries(cameratest PRIVATE GTest::gtest_main CameraCore CameraNull)
include(GoogleTest)
gtest_discover_tests(cameratest)

if(WIN32)
    add_custom_command(TARGET cameratest POST_BUILD
        COMMAND  -E copy_if_different
        \$<TARGET_FILE:CameraCore>
        \$<TARGET_FILE_DIR:cameratest>
    )
    add_custom_command(TARGET cameratest POST_BUILD
        COMMAND  -E copy_if_different
        \$<TARGET_FILE:CameraNull>
        \$<TARGET_FILE_DIR:cameratest>
    )
endif()
gtest_discover_tests(cameratest)

