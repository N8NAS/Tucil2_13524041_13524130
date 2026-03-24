cmake_minimum_required(VERSION 3.10)
project(VoxelProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include headers
include_directories(src/include)

# executable
add_executable(voxel_gen src/voxelgenerator.cpp)

# # Object_test/default
# set(INPUT_FILE "teapot.obj" CACHE STRING "Nama file OBJ input di folder test")
# set(OUTPUT_FILE "output.obj" CACHE STRING "Nama file OBJ output di folder bin")

# # costum command for voxelisation
# add_custom_command(
#     OUTPUT ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_FILE}
#     COMMAND voxel_gen ${CMAKE_SOURCE_DIR}/test/${INPUT_FILE} ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_FILE}
#     DEPENDS voxel_gen
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
#     COMMENT "Generating ${OUTPUT_FILE} from test/${INPUT_FILE}"
#     VERBATIM
# )

# # obj generator
# add_custom_target(generate_obj
#     DEPENDS ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_FILE}
# )

# blender
add_custom_target(view
    COMMAND blender --python ${CMAKE_SOURCE_DIR}/src/import_obj.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Launching Blender to view the latest bin/output.obj"
)