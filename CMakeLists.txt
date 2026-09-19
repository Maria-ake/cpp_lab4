cmake_minimum_required(VERSION 3.10)

if(NOT DEFINED PATCH_VERSION)
    set(PATCH_VERSION 0)
endif()

project(lab4 VERSION 1.0.${PATCH_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab4 src/main.cpp)
target_include_directories(lab4 PRIVATE include)