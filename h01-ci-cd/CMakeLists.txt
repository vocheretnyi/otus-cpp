add_executable(h01 main.cpp)

add_definitions(-DPATCH_VERSION=${PATCH_VERSION})

if (ENABLE_TESTING)
    add_executable(h01-tests version_tests.cpp)
    target_link_libraries(h01-tests PRIVATE  gtest)
#    target_compile_definitions(h01-tests PUBLIC -DRESOURCES_PATH="${RESOURCES_PATH}")
    add_test(h01-tests h01-tests)
endif()