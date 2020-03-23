cmake_minimum_required(VERSION 3.10)

project(test_run)

set(SOURCES 
    ${PROJECT_SOURCE_DIR}/main_test.cpp
    ${PROJECT_SOURCE_DIR}/splay_test.cpp
    )

find_package(GTest REQUIRED)

add_executable(test_run ${SOURCES})
target_link_libraries(test_run ${GTEST_LIBRARIES} pthread)
target_link_libraries(test_run splay)

set_target_properties(test_run PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO
                      )
