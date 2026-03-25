option(CAMERA_ENABLE_SANITIZERS "Enable sanitizers for debug builds" OFF)

function(camera_enable_sanitizers target_name)
    if(NOT CAMERA_ENABLE_SANITIZERS)
        return()
    endif()

    if(MSVC)
        message(STATUS "Sanitizers are currently configured for non-MSVC toolchains only")
        return()
    endif()

    target_compile_options(${target_name} PRIVATE -fsanitize=address,undefined)
    target_link_options(${target_name} PRIVATE -fsanitize=address,undefined)
endfunction()
