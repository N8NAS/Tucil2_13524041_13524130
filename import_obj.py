import bpy
import os

bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete()

file_path = os.path.join(os.getcwd(), "output.obj")

if os.path.exists(file_path):
    bpy.ops.wm.obj_import(filepath=file_path)