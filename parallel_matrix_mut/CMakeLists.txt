cmake_minimum_required(VERSION 3.19)
project(parallel_matrix_mut CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(HPX REQUIRED)

add_executable(matrix_mul main.cpp)

target_link_libraries(matrix_mul PRIVATE HPX::hpx HPX::wrap_main)

hpx_setup_target(matrix_mul)