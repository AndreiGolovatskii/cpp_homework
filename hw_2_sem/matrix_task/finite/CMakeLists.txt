cmake_minimum_required(VERSION 3.10)

project(finite)

set(SOURCES ${PROJECT_SOURCE_DIR}/finite.h)

add_library(finite ${SOURCES})
target_link_libraries(finite type_traits)

set_target_properties(finite PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO
                      )

