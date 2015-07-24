import bpy
import os

filename = os.path.join(os.path.dirname(bpy.data.filepath), "../../../tools/blenderexport/Export.py")
print(filename)
exec(compile(open(filename).read(), filename, 'exec'))