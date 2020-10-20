cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(process STATIC src/process.cpp)
target_include_directories(process PUBLIC include)

add_executable(example example/example.cpp)
target_link_libraries(example process)
