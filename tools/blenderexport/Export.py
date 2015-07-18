import bpy
import os
import xml.etree.cElementTree as ET

def Vector3ToString( obj ):
   return "[" + str(obj.x) + "," + str(obj.y) + "," + str(obj.z) + "]"

def ConvertMesh( mesh ):
    if len(mesh.vertices) == 0:
        return
    print("Converting mesh " + mesh.name)
    meshEle = ET.SubElement(meshesEle, "Mesh")
    meshEle.set("name", mesh.name)
    vertexesEle = ET.SubElement(meshEle, "Vertexes")
    for vertex in mesh.vertices:
        vertexDataEle = ET.SubElement(vertexesEle, "VertexData")
        vertexDataEle.set("normal", Vector3ToString(vertex.normal))
        vertexDataEle.set("position", Vector3ToString(vertex.co))
        
def ConvertEmpty(object):
    return

def ConvertObject(object, scene):
    print("Converting type " + object.type)
    return {
        'EMPTY': ConvertEmpty(object),
        'MESH': ConvertMesh(object.data),
        'LAMP' : ConvertEmpty(object),
    }[object.type]
    
def ConvertScene(scene):
    for object in scene.objects:
        ConvertObject(object, scene)    
    
def ConvertScenes():
    for scene in bpy.data.objects.data.scenes:
        ConvertScene(scene) 

# export to blend file location
basedir = os.path.dirname(bpy.data.filepath)

if not basedir:
    raise Exception("Blend file is not saved")

filename = os.path.splitext(os.path.basename(bpy.data.filepath))[0]
    
appEle = ET.Element("App")
resourcesEle = ET.SubElement(appEle, "Resources")
meshesEle = ET.SubElement(appEle, "Meshes")

ConvertScenes()

tree = ET.ElementTree(appEle)
tree.write(basedir + "/" + filename + ".xml")