cmake_minimum_required(VERSION 3.22)
project(parallel_fib)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

add_executable(parallel-fib main.cpp)
target_link_libraries(parallel-fib PUBLIC OpenMP::OpenMP_CXX pthread)

MATH(EXPR stack_size "512 * 1024 * 1024") # 16 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")