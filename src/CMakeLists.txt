cmake_minimum_required(VERSION 3.14)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(src ${SOURCES})
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src})
    target_compile_options(${name} PRIVATE -fopenmp)
    target_link_libraries(${name} PRIVATE OpenMP::OpenMP_CXX)
endforeach()