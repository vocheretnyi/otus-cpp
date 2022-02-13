#add_executable(h03 main.cpp)
#
#install(TARGETS h03 RUNTIME DESTINATION bin)

if (ENABLE_TESTING)
    add_executable(allocator_tests allocator_tests.cpp)
    target_link_libraries(allocator_tests PRIVATE gtest)
    add_test(h03-tests allocator_tests)
endif()