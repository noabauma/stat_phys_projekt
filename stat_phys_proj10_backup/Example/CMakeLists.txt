cmake_minimum_required(VERSION 3.0)

project(StatPhysQCExample)

set (CMAKE_CXX_STANDARD 11)

find_package(StatPhysQC)

add_executable(example test.cpp)
target_link_libraries(example StatPhysQC::StatPhysQC)
