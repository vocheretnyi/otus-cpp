#set(CMAKE_CXX_STANDARD 17)

add_executable(h03 main.cpp)

if (ENABLE_TESTING)
    add_executable(allocator_tests allocator_tests.cpp)
    target_link_libraries(allocator_tests PRIVATE gtest)
    add_test(h03-tests allocator_tests)
endif()