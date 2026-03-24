import bpy
import os

bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete()

cwd = os.getcwd()
file_path = os.path.join(cwd, "test", "output.obj")

if os.path.exists(file_path):
    try:
        bpy.ops.wm.obj_import(filepath=file_path)
        # print(f"Berhasil mengimpor: {file_path}")
    except AttributeError:
        bpy.ops.import_scene.obj(filepath=file_path)
else:
    print(f"Error: path file error{file_path}")