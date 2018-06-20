cmake_minimum_required(VERSION 3.4)
project(scsh)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SRCs AST.cpp
         Util.cpp
         scsh.cpp)
set(CMAKE_CXX_FLAGS "-g -std=c++11")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(/usr/local/lib)

add_executable(scsh ${SRCs})
target_link_libraries(scsh gtest)

add_subdirectory(test)