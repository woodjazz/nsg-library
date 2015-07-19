import bpy
import os
import xml.etree.cElementTree as et


def Vector3ToString(obj):
	return '[{: f},{: f},{: f}]'.format(obj.x, obj.y, obj.z)

def Vector2ToString(obj):
    return '[{: f},{: f}]'.format(obj.x, obj.y)

def ColorToString(obj):
    return '[{: f},{: f},{: f}]'.format(obj.r, obj.g, obj.b)

def ConvertColors(vertexesEle, mesh):

    total_vc_channels = len(mesh.tessface_vertex_colors)

    if total_vc_channels > 2:
        total_vc_channels = 2

    # Loop though each of the VC channels
    for loop in range(total_vc_channels):
        vc_channel = mesh.tessface_vertex_colors[loop]
        for poly_num in range(len(mesh.tessfaces)):
            # Loop though each polygon
            poly = mesh.tessfaces[poly_num]
            if len(poly.vertices) == 4:
                vertexesEle[poly.vertices[0]].set(
                    "color", ColorToString(vc_channel.data[poly_num].color1))
                vertexesEle[poly.vertices[1]].set(
                    "color", ColorToString(vc_channel.data[poly_num].color2))
                vertexesEle[poly.vertices[2]].set(
                    "color", ColorToString(vc_channel.data[poly_num].color3))
                vertexesEle[poly.vertices[2]].set(
                    "color", ColorToString(vc_channel.data[poly_num].color3))
                vertexesEle[poly.vertices[0]].set(
                    "color", ColorToString(vc_channel.data[poly_num].color1))
                vertexesEle[poly.vertices[3]].set(
                    "color", ColorToString(vc_channel.data[poly_num].color4))
            elif len(poly.vertices) == 3:
                vertexesEle[poly.vertices[0]].set(
                    "color", ColorToString(vc_channel.data[poly_num].color1))
                vertexesEle[poly.vertices[1]].set(
                    "color", ColorToString(vc_channel.data[poly_num].color2))
                vertexesEle[poly.vertices[2]].set(
                    "color", ColorToString(vc_channel.data[poly_num].color3))


def ConvertUV(vertexesEle, mesh):

    total_uv_channels = len(mesh.tessface_uv_textures)

    if total_uv_channels > 2:
        total_uv_channels = 2

    # Loop through each of the UV channels
    for loop in range(total_uv_channels):
        key = "uv" + str(loop)
        uv_channel = mesh.tessface_uv_textures[loop]

        # save.strings(uv_channel.data[0].image.name)
        # for this channel# Channels name
        for poly_num in range(len(mesh.tessfaces)):
            # Loop though each polygon
            poly = mesh.tessfaces[poly_num]
            if len(poly.vertices) == 4:
                vertexesEle[poly.vertices[0]].set(
                    key, Vector2ToString(uv_channel.data[poly_num].uv1))
                vertexesEle[poly.vertices[1]].set(
                    key, Vector2ToString(uv_channel.data[poly_num].uv2))
                vertexesEle[poly.vertices[2]].set(
                    key, Vector2ToString(uv_channel.data[poly_num].uv3))
                vertexesEle[poly.vertices[2]].set(
                    key, Vector2ToString(uv_channel.data[poly_num].uv3))
                vertexesEle[poly.vertices[0]].set(
                    key, Vector2ToString(uv_channel.data[poly_num].uv2))
                vertexesEle[poly.vertices[3]].set(
                    key, Vector2ToString(uv_channel.data[poly_num].uv4))
            elif len(poly.vertices) == 3:
                vertexesEle[poly.vertices[0]].set(
                    key, Vector2ToString(uv_channel.data[poly_num].uv1))
                vertexesEle[poly.vertices[1]].set(
                    key, Vector2ToString(uv_channel.data[poly_num].uv2))
                vertexesEle[poly.vertices[2]].set(
                    key, Vector2ToString(uv_channel.data[poly_num].uv3))


def ConvertMesh(mesh):
    if len(mesh.vertices) == 0:
        return

    print("Converting mesh " + mesh.name)

    mesh.update(calc_tessface=True)

    meshEle = et.SubElement(meshesEle, "Mesh")

    meshEle.set("name", mesh.name)

    index = 0
    for uv in mesh.tessface_uv_textures:
        key = "uv" + str(index) + "Name"
        meshEle.set(key, uv.name)
        index += 1

    vertexesEle = et.SubElement(meshEle, "Vertexes")
    for vertex in mesh.vertices:
        vertexDataEle = et.SubElement(vertexesEle, "VertexData")
        vertexDataEle.set("position", Vector3ToString(vertex.co))
        vertexDataEle.set("normal", Vector3ToString(vertex.normal))
        total_groups = len(vertex.groups)
        if total_groups > 4:
        	total_groups = 4
        bonesID = ()
        bonesWeight = ()
        for i in range(total_groups):
        	group = vertex.groups[i]
        	bonesID = bonesID + (group.group, )
        	bonesWeight = bonesWeight + (group.weight, )
        if len(bonesID) > 0:
        	vertexDataEle.set("bonesID", bonesID)
        	vertexDataEle.set("bonesWeight", bonesWeight)


    indexesEle = et.SubElement(meshEle, "Indexes")
    indexes = ""
    for face in mesh.tessfaces:
        v = face.vertices
        if len(v) == 4:
            indexes = indexes + '{:d} {:d} {:d} {:d} {:d} {:d} '.format(v[0], v[1], v[2], v[2], v[0], v[3])
        else:
            indexes = indexes + '{:d} {:d} {:d} '.format(v[0], v[1], v[2])
    indexesEle.text = indexes

    ConvertColors(vertexesEle, mesh)
    ConvertUV(vertexesEle, mesh)


def ConvertMeshes():
    for mesh in bpy.data.objects.data.meshes:
        ConvertMesh(mesh)


def ConvertEmpty(object):
    return


def ConvertObject(object, scene):
    print("Converting type " + object.type)
    return {
        'EMPTY': ConvertEmpty(object),
        'MESH': ConvertMesh(object.data),
        'LAMP': ConvertEmpty(object),
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

appEle = et.Element("App")
resourcesEle = et.SubElement(appEle, "Resources")
meshesEle = et.SubElement(appEle, "Meshes")

ConvertMeshes()

tree = et.ElementTree(appEle)
tree.write(basedir + "/" + filename + ".xml")
