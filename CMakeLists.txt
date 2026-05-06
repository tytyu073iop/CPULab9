cmake_minimum_required(VERSION 3.14)

if(APPLE)
    set(CMAKE_C_COMPILER /opt/homebrew/opt/gcc/bin/gcc-15)
    set(CMAKE_CXX_COMPILER /opt/homebrew/opt/gcc/bin/g++-15)
    set(OpenMP_CXX_LIB_NAMES "gomp")
    set(OpenMP_CXX_FLAGS "-fopenmp")
    set(OpenMP_CXX_INCLUDE_DIR "/opt/homebrew/opt/gcc/include")
    find_library(OpenMP_gomp_LIBRARY NAMES gomp PATHS /opt/homebrew/lib/gcc/15)
elseif(UNIX)
    set(OpenMP_CXX_FLAGS "-fopenmp")
endif()

project(CPULab9 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)

add_subdirectory(src)