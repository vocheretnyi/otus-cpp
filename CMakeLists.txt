cmake_minimum_required(VERSION 3.19)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

message("PROJECT_VERSION: ${PROJECT_VERSION}")

project(otus-cpp VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)

# options
option(ENABLE_TESTING "Whether to build test" ON)
if (ENABLE_TESTING)
    enable_testing()
endif()

add_compile_options(-Wall -Wextra -pedantic -Werror)

# include libs dir
add_subdirectory(libs)

# include homeworks
add_subdirectory(h01-ci-cd)
add_subdirectory(h02-ip-filter)

# include directory with timus problems
add_subdirectory(timus EXCLUDE_FROM_ALL)

# package
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT test@example.com)
include(CPack)