cmake_minimum_required(VERSION 3.10)
project(VoxelProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(voxel_gen voxeltest.cpp)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/output.obj
    COMMAND voxel_gen
    DEPENDS voxel_gen
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating output.obj from voxeltest.cpp"
)

add_custom_target(generate_obj ALL
    DEPENDS ${CMAKE_BINARY_DIR}/output.obj
)
add_custom_target(view
    COMMAND blender --python ${CMAKE_SOURCE_DIR}/import_obj.py
    DEPENDS generate_obj
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Launching Blender to view geometry"
)