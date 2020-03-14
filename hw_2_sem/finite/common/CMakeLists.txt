cmake_minimum_required(VERSION 3.10)

project(type_traits)

set(SOURCES ${PROJECT_SOURCE_DIR}/type_traits.h)

add_library(type_traits SHARED ${SOURCES})

set_target_properties(type_traits PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO
                      )


find_package(GTest REQUIRED)

add_executable(traits_test ${PROJECT_SOURCE_DIR}/tests.cpp)

target_link_libraries(traits_test type_traits)
target_link_libraries(traits_test ${GTEST_LIBRARIES} pthread)

set_target_properties(traits_test PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO
                      )
