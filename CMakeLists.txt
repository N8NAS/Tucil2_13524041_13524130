cmake_minimum_required(VERSION 3.10)
project(VoxelProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#package
find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
#include
include_directories(src/include)
include_directories(src)

#executable
add_executable(voxel_gen src/voxelgenerator.cpp)

#link library
target_link_libraries(voxel_gen PRIVATE 
    Threads::Threads 
    SDL2::SDL2main   
    SDL2::SDL2 
)
