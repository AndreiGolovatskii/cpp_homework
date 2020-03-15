cmake_minimum_required(VERSION 3.10)

project(biginteger)

set(SOURCES ${PROJECT_SOURCE_DIR}/biginteger.h
            ${PROJECT_SOURCE_DIR}/rational.h
            ${PROJECT_SOURCE_DIR}/fast_fourier_transform.cpp
    )

add_library(biginteger SHARED ${SOURCES})

set_target_properties(biginteger PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO
                      )

