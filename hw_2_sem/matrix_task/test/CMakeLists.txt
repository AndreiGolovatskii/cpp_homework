cmake_minimum_required(VERSION 3.10)

project(test_run)

set(SOURCES 
    ${PROJECT_SOURCE_DIR}/main_test.cpp
    ${PROJECT_SOURCE_DIR}/type_traits_test.cpp 
    ${PROJECT_SOURCE_DIR}/finite_test.cpp
    ${PROJECT_SOURCE_DIR}/matrix_test.cpp
    )

find_package(GTest REQUIRED)

add_executable(test_run ${SOURCES})
target_link_libraries(test_run ${GTEST_LIBRARIES} pthread)
target_link_libraries(test_run type_traits)
target_link_libraries(test_run finite)
