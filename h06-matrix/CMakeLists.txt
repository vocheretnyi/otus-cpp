add_library(matrix matrix.cpp)
find_package(Boost REQUIRED)
# PUBLIC - because we need inherit the boost includes.
target_include_directories(matrix PUBLIC ${Boost_INCLUDE_DIR})

add_executable(h06 main.cpp)
target_link_libraries(h06 PRIVATE matrix)

if (ENABLE_TESTING)
    add_executable(matrix_tests tests.cpp)
    target_link_libraries(matrix_tests PRIVATE gtest PRIVATE matrix)

    # Crunch which allows using `ASSERT_EQ(obj.size(), 0);`
    # Which means comparing unsigned and signed numbers (0 - by default is int).
    #
    target_compile_options(matrix_tests PRIVATE -Wno-sign-compare)
    add_test(h06-tests matrix_tests)
endif()