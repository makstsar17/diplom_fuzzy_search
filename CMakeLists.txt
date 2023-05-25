cmake_minimum_required(VERSION 3.25)
project(diplom_last)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)
add_executable(diplom_last main.cpp)
target_link_libraries(diplom_last similarityTable)
