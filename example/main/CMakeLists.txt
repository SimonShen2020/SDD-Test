add_executable(example_main main.cpp)

target_link_libraries(example_main PRIVATE CameraCore CameraNull)

# Copy config file to build directory
add_custom_command(TARGET example_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/config.json
    $<TARGET_FILE_DIR:example_main>/config.json
)
