cmake_minimum_required(VERSION 3.10)

project(splay)

set(SOURCES 
    ${PROJECT_SOURCE_DIR}/splay.h
    )

add_library(splay ${SOURCES})

set_target_properties(splay PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO
                      )
