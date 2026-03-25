include(FetchContent)

# Pull GoogleTest for unit/integration tests.
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

function(camera_ensure_gtest)
    if(NOT TARGET GTest::gtest)
        FetchContent_MakeAvailable(googletest)
    endif()
endfunction()
