cmake_minimum_required(VERSION 3.10)

project(type_traits)

set(SOURCES ${PROJECT_SOURCE_DIR}/type_traits.h)

add_library(type_traits ${SOURCES})

set_target_properties(type_traits PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO
                      )
