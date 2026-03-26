# Shared compiler options for the camera project.
function(camera_apply_default_compiler_options target_name)
    target_compile_features(${target_name} PUBLIC cxx_std_17)

    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4 /permissive- /utf-8)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()
