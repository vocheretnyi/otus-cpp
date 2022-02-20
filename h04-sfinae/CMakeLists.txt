#set(CMAKE_CXX_STANDARD 20)

#set(RESOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/rsc)

# create Ip Filter library.
#add_library(ip_filter ip_address.cpp ip_filter.cpp)
#target_include_directories(ip_filter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(h04 main.cpp)
target_compile_options(h04 PRIVATE -Wno-shift-count-overflow)
#target_compile_definitions(h02 PUBLIC -DRESOURCES_PATH="${RESOURCES_PATH}")
#target_link_libraries(h02 PRIVATE ip_filter)

#if (ENABLE_TESTING)
#    add_executable(ip_filer_tests ip_filer_tests.cpp)
#    target_link_libraries(ip_filer_tests PRIVATE gtest ip_filter OpenSSL::SSL)
#    target_compile_definitions(ip_filer_tests PUBLIC -DRESOURCES_PATH="${RESOURCES_PATH}")
#    target_compile_options(ip_filer_tests PRIVATE -Wno-deprecated) # because of OpenSSL MD5
#    add_test(h02-tests ip_filer_tests)
#endif()