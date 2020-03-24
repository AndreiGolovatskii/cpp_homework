cmake_minimum_required(VERSION 3.10)

project(hw_2_sem)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wnon-virtual-dtor -pedantic")

include_directories(AFTER ${PROJECT_SOURCE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/splay)
add_subdirectory(${PROJECT_SOURCE_DIR}/test)

add_executable(main ${PROJECT_SOURCE_DIR}/task_contest/main.cpp)
