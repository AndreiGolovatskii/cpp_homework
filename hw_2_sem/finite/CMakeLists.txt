cmake_minimum_required(VERSION 3.10)

project(finite)

add_subdirectory(${PROJECT_SOURCE_DIR}/common)

set(SOURCES ${PROJECT_SOURCE_DIR}/finite.h)

add_library(finite SHARED ${SOURCES})
target_link_libraries(finite type_traits)

set_target_properties(finite PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO
                      )

find_package(GTest REQUIRED)

add_executable(finite_test ${PROJECT_SOURCE_DIR}/tests.cpp)

target_link_libraries(finite_test finite)
target_link_libraries(finite_test ${GTEST_LIBRARIES} pthread)

set_target_properties(finite_test PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO
                      )

