add_executable(h01 main.cpp)

add_definitions(-DPATCH_VERSION=${PATCH_VERSION})

if (ENABLE_TESTING)
    add_executable(h01-tests version_tests.cpp)
    target_link_libraries(h01-tests PRIVATE gtest)
    add_test(h01-tests h01-tests)
endif()

install(TARGETS h01 RUNTIME DESTINATION bin)