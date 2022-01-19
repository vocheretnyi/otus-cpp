cmake_minimum_required(VERSION 3.19)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

message("PROJECT_VERSION: ${PROJECT_VERSION}")

project(otus-cpp VERSION ${PROJECT_VERSION})

# Options
option(ENABLE_TESTING "Whether to build test" ON)
if (ENABLE_TESTING)
    enable_testing()
endif()

add_subdirectory(libs)

add_subdirectory(h01-ci-cd)

# package
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT test@example.com)
include(CPack)