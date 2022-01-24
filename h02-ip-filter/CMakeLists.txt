set(CMAKE_CXX_STANDARD 20)
set(RESOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/rsc)

add_library(ip_filter ip_address.cpp ip_filter.cpp)
message(${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ip_filter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(h02 main.cpp)
target_compile_definitions(h02 PUBLIC -DRESOURCES_PATH="${RESOURCES_PATH}")

# TODO: change, as personal mac specific
set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
find_package(OpenSSL REQUIRED)

target_link_libraries(h02 PRIVATE ip_filter)

if (ENABLE_TESTING)
    add_executable(ip_filer_tests ip_filer_tests.cpp)
    target_link_libraries(ip_filer_tests PRIVATE gtest ip_filter OpenSSL::SSL)
    target_compile_definitions(ip_filer_tests PUBLIC -DRESOURCES_PATH="${RESOURCES_PATH}")
    target_compile_options(ip_filer_tests PRIVATE -Wno-deprecated) # because of OpenSSL MD5
    add_test(h02-tests ip_filer_tests)
endif()