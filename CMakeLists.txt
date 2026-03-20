cmake_minimum_required(VERSION 3.10)
project(VoxelProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Output executable ke folder bin di root project
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 2. Include headers
include_directories(src/include)

# 3. Build executable
add_executable(voxel_gen src/voxelgenerator.cpp)

# 4. Variabel Input File (Bisa diganti lewat terminal)
# Default: teapot.obj
set(INPUT_FILE "teapot.obj" CACHE STRING "Nama file OBJ di folder test")

# 5. Generate output.obj di dalam folder bin
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/bin/output.obj
    COMMAND voxel_gen ${CMAKE_SOURCE_DIR}/test/${INPUT_FILE}
    DEPENDS voxel_gen
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    COMMENT "Generating output.obj from test/${INPUT_FILE}"
    VERBATIM
)

# 6. Target untuk trigger generasi file secara otomatis
add_custom_target(generate_obj ALL
    DEPENDS ${CMAKE_SOURCE_DIR}/bin/output.obj
)

# 7. Target untuk meluncurkan Blender
add_custom_target(view
    COMMAND blender --python ${CMAKE_SOURCE_DIR}/src/import_obj.py
    DEPENDS generate_obj
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Launching Blender to view geometry"
)