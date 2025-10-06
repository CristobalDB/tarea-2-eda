cmake_minimum_required(VERSION 3.16)
project(poscodes LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Paths
include_directories(${PROJECT_SOURCE_DIR}/include)

# Sources
add_executable(test
    test.cpp
    src/utils.cpp
    src/poscode.cpp
    src/LinkedList.cpp
    src/CountingSort.cpp
    src/RadixSort.cpp
    src/QuickSort.cpp
    src/MergeSort.cpp
)
