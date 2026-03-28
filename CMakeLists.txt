cmake_minimum_required(VERSION 3.10)
project(VoxelProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Package
find_package(Threads REQUIRED)
find_package(X11 REQUIRED)

# Include
include_directories(src/include)
include_directories(src)
include_directories(${X11_INCLUDE_DIR})

# Executable
add_executable(voxel_gen src/voxelgenerator.cpp)

# Link library
target_link_libraries(voxel_gen PRIVATE 
    Threads::Threads 
    ${X11_LIBRARIES}
)

