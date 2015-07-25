import bpy
import mathutils
import math
import os
import xml.etree.cElementTree as et


def Clamp(value, min_value, max_value):
    return max(min(value, max_value), min_value)


def BoolToString(obj):
    if obj:
        return "true"
    else:
        return "false"


def FloatToString(obj):
    return '{:g}'.format(obj)


def QuaternionToString(obj):
    return '[{:g},{:g},{:g},{:g}]'.format(obj.w, obj.x, obj.y, obj.z)


def Vector4ToString(obj):
    return '[{:g},{:g},{:g},{:g}]'.format(obj.x, obj.y, obj.z, obj.w)


def Matrix4ToString(obj):
    return '[{:s} {:s} {:s} {:s}]'.format(Vector4ToString(obj[0]),
                                          Vector4ToString(obj[1]),
                                          Vector4ToString(obj[2]),
                                          Vector4ToString(obj[3]))


def Vector3ToString(obj):
    return '[{:g},{:g},{:g}]'.format(obj.x, obj.y, obj.z)


def Vector2ToString(obj):
    return '[{:g},{:g}]'.format(obj.x, obj.y)


def ColorToString(obj):
    return '[{:g},{:g},{:g}]'.format(obj.r, obj.g, obj.b)


def BoundTypeToString(bound):
    if bound == 'BOX':
        return "SH_BOX"
    elif bound == 'SPHERE':
        return "SH_SPHERE"
    elif bound == 'CYLINDER':
        return "SH_CYLINDER"
    elif bound == 'CONE':
        return "SH_CONE"
    elif bound == 'CONVEX_HULL':
        return "SH_CONVEX_TRIMESH"
    elif bound == 'TRIANGLE_MESH':
        return "SH_TRIMESH"
    elif bound == 'CAPSULE':
        return "SH_CAPSULE"
    else:
        return "SH_UNKNOWN"


def LightTypeToString(lightType):
    if lightType == 'SUN':
        return "DIRECTIONAL"
    elif lightType == 'SPOT':
        return "SPOT"
    else:
        return "POINT"


def SensorFitToString(sensorFit):
    if sensorFit == 'AUTO':
        return "0"
    elif sensorFit == 'HORIZONTAL':
        return "1"
    else:
        return "2"


def GetPhysicShapeName(obj):
    loc, rot, scale = obj.matrix_world.decompose()
    scaleStr = Vector3ToString(scale)
    typeStr = BoundTypeToString(obj.game.collision_bounds_type)
    name = scaleStr + " " + typeStr
    return name, scaleStr, typeStr


def CreatePhysics(sceneNodeEle, obj):
    if obj and obj.game.physics_type != 'NO_COLLISION':
        rigidBodyEle = et.SubElement(sceneNodeEle, "RigidBody")

        if obj.game.physics_type == 'STATIC':
            rigidBodyEle.set("mass", "0")
        else:
            rigidBodyEle.set("mass", FloatToString(obj.game.mass))

        if obj.game.physics_type == 'SENSOR':
            rigidBodyEle.set("trigger", BoolToString(True))
        else:
            rigidBodyEle.set("trigger", BoolToString(False))

        if obj.game.physics_type == 'CHARACTER':
            rigidBodyEle.set("kinematic", BoolToString(True))
        else:
            rigidBodyEle.set("kinematic", BoolToString(False))

        rigidBodyEle.set("linearDamp", FloatToString(obj.game.damping))
        rigidBodyEle.set(
            "angularDamp", FloatToString(obj.game.rotation_damping))

        linearFactor = mathutils.Vector((1, 1, 1))
        if obj.game.lock_location_x:
            linearFactor.x = 0
        if obj.game.lock_location_y:
            linearFactor.y = 0
        if obj.game.lock_location_z:
            linearFactor.z = 0
        rigidBodyEle.set("linearFactor", Vector3ToString(linearFactor))

        angularFactor = mathutils.Vector((1, 1, 1))
        if obj.game.lock_rotation_x:
            angularFactor.x = 0
        if obj.game.lock_rotation_y:
            angularFactor.y = 0
        if obj.game.lock_rotation_z:
            angularFactor.z = 0
        rigidBodyEle.set("angularFactor", Vector3ToString(angularFactor))
        return rigidBodyEle


QUATERNION_IDENTITY = mathutils.Quaternion()
QUATERNION_IDENTITY.identity()
VECTOR3_ZERO = mathutils.Vector((0, 0, 0))
VECTOR3_ONE = mathutils.Vector((1, 1, 1))


def CreateSceneNode(name, parentElem, obj, loc=None, rot=None, sca=None):
    sceneNodeEle = et.SubElement(parentElem, "SceneNode")
    sceneNodeEle.set("name", name)
    if loc:
        sceneNodeEle.set("position", Vector3ToString(loc))
    else:
        sceneNodeEle.set(
            "position", Vector3ToString(VECTOR3_ZERO))
    if rot:
        sceneNodeEle.set("orientation", QuaternionToString(rot))
    else:
        sceneNodeEle.set(
            "orientation", QuaternionToString(QUATERNION_IDENTITY))

    if sca:
        sceneNodeEle.set("scale", Vector3ToString(sca))
    else:
        sceneNodeEle.set(
            "scale", Vector3ToString(VECTOR3_ONE))

    CreatePhysics(sceneNodeEle, obj)

    return sceneNodeEle


def ConvertColors(meshData, mesh):
    total_vc_channels = Clamp(len(mesh.tessface_vertex_colors), 0, 1)
    for channel in range(total_vc_channels):
        vc_channel = mesh.tessface_vertex_colors[channel]
        for face_num in range(len(mesh.tessfaces)):
            face = mesh.tessfaces[face_num]
            nVertex = len(face.vertices)
            assert nVertex == 4 or nVertex == 3
            meshData[face.vertices[0]]["color"] = ColorToString(
                vc_channel.data[face_num].color1)
            meshData[face.vertices[1]]["color"] = ColorToString(
                vc_channel.data[face_num].color2)
            meshData[face.vertices[2]]["color"] = ColorToString(
                vc_channel.data[face_num].color3)
            if nVertex == 4:
                meshData[face.vertices[3]]["color"] = ColorToString(
                    vc_channel.data[face_num].color4)


def ConvertUV(meshData, mesh):
    total_uv_channels = Clamp(len(mesh.tessface_uv_textures), 0, 2)
    for channel in range(total_uv_channels):
        key = "uv" + str(channel)
        uv_channel = mesh.tessface_uv_textures[channel]
        for face_num in range(len(mesh.tessfaces)):
            face = mesh.tessfaces[face_num]
            nVertex = len(face.vertices)
            assert nVertex == 4 or nVertex == 3
            meshData[face.vertices[0]][key] = Vector2ToString(
                uv_channel.data[face_num].uv1)
            meshData[face.vertices[1]][key] = Vector2ToString(
                uv_channel.data[face_num].uv2)
            meshData[face.vertices[2]][key] = Vector2ToString(
                uv_channel.data[face_num].uv3)
            if nVertex == 4:
                meshData[face.vertices[3]][key] = Vector2ToString(
                    uv_channel.data[face_num].uv4)


def ConvertUVMaps(meshEle, mesh):
    for index, uv in enumerate(mesh.tessface_uv_textures):
        key = "uv" + str(index) + "Name"
        meshEle.set(key, uv.name)


def ConvertBonesWeigths(vertexData, vertex):
    total_groups = Clamp(len(vertex.groups), 0, 4)
    bonesID = mathutils.Vector((0.0, 0.0, 0.0, 0.0))
    bonesWeight = mathutils.Vector((0.0, 0.0, 0.0, 0.0))
    for i in range(total_groups):
        group = vertex.groups[i]
        bonesID[i] = group.group
        bonesWeight[i] = group.weight
    if total_groups > 0:
        vertexData["bonesID"] = Vector4ToString(bonesID)
        vertexData["bonesWeight"] = Vector4ToString(bonesWeight)


def AddVertex(vertexesEle, meshData, i):
    vertexDataEle = et.SubElement(vertexesEle, "VertexData")
    vertexDataEle.set("position", meshData[i]["position"])
    vertexDataEle.set("normal", Vector3ToString(meshData[i]["normal"]))
    if "uv0" in meshData[i]:
        vertexDataEle.set("uv0", meshData[i]["uv0"])
    if "uv1" in meshData[i]:
        vertexDataEle.set("uv1", meshData[i]["uv1"])
    if "bonesID" in meshData[i]:
        vertexDataEle.set("bonesID", meshData[i]["bonesID"])
    if "bonesWeight" in meshData[i]:
        vertexDataEle.set("bonesWeight", meshData[i]["bonesWeight"])


def AverageNormals(meshData, indexes):
    normal = VECTOR3_ZERO
    for i in indexes:
        normal += meshData[i]["normal"]
        normal /= len(indexes)
    return normal


def AddQuad(vertexesEle, meshData, i0, i1, i2, i3, i4, i5):
    AverageNormals(meshData, [i0, i1, i2, i5])
    AddVertex(vertexesEle, meshData, i0)
    AddVertex(vertexesEle, meshData, i1)
    AddVertex(vertexesEle, meshData, i2)
    AddVertex(vertexesEle, meshData, i3)
    AddVertex(vertexesEle, meshData, i4)
    AddVertex(vertexesEle, meshData, i5)


def AddTriangle(vertexesEle, meshData, i0, i1, i2):
    AverageNormals(meshData, [i0, i1, i2])
    AddVertex(vertexesEle, meshData, i0)
    AddVertex(vertexesEle, meshData, i1)
    AddVertex(vertexesEle, meshData, i2)


def GenerateVertexes(meshData, meshEle, mesh, materialIndex, vertexesEle):
    indexesEle = et.SubElement(meshEle, "Indexes")
    indexes = ""
    i = 0
    for face in mesh.tessfaces:
        if face.material_index == materialIndex:
            v = face.vertices
            if len(v) == 4:
                indexes = indexes + '{:d} {:d} {:d} {:d} {:d} {:d} '.format(
                    i, i+1, i+2, i+3, i+4, i+5)
                i = i + 6
                AddQuad(vertexesEle, meshData, v[0], v[1], v[2], v[0], v[2], v[3])
            else:
                indexes = indexes + '{:d} {:d} {:d} '.format(i, i+1, i+2)
                i = i + 3
                AddTriangle(vertexesEle, meshData, v[0], v[1], v[2])
    indexesEle.text = indexes


def ConvertMesh(name, meshesEle, mesh, materialIndex):
    if len(mesh.vertices) == 0:
        return
    print("Converting mesh " + mesh.name)
    mesh.update(calc_tessface=True)
    meshEle = et.SubElement(meshesEle, "Mesh")
    meshEle.set("name", name)
    ConvertUVMaps(meshEle, mesh)
    indexes = ""
    i = 0
    vertexesEle = et.SubElement(meshEle, "Vertexes")
    indexesEle = et.SubElement(meshEle, "Indexes")
    for iface, face in enumerate(mesh.tessfaces):
        assert len(face.vertices) == 4 or len(face.vertices) == 3
        if len(face.vertices) == 4:
            indexes = indexes + '{:d} {:d} {:d} {:d} {:d} {:d} '.format(i, i+1, i+2, i, i+2, i+3)
        else:
            indexes = indexes + '{:d} {:d} {:d} '.format(i, i+1, i+2)
        i += len(face.vertices)
        vertexNum = 0
        for ivertex in face.vertices:      
            vertex = mesh.vertices[ivertex]
            vertexData = {}
            vertexData["position"] = Vector3ToString(vertex.co)
            if face.use_smooth:
                vertexData["normal"] = Vector3ToString(vertex.normal)
            else:
                vertexData["normal"] = Vector3ToString(face.normal)
            ConvertBonesWeigths(vertexData, vertex)

            channels = Clamp(len(mesh.tessface_uv_textures), 0, 2)
            for channel in range(channels):
                uv_channel = mesh.tessface_uv_textures[channel]
                key = "uv" + str(channel)
                if vertexNum == 0:
                    vertexData[key] = Vector2ToString(uv_channel.data[iface].uv1)
                elif vertexNum == 1:
                    vertexData[key] = Vector2ToString(uv_channel.data[iface].uv2)
                elif vertexNum == 2:
                    vertexData[key] = Vector2ToString(uv_channel.data[iface].uv3)
                elif vertexNum == 3:
                    vertexData[key] = Vector2ToString(uv_channel.data[iface].uv4)

            channels = Clamp(len(mesh.tessface_vertex_colors), 0, 1)
            for channel in range(channels):
                vc_channel = mesh.tessface_vertex_colors[channel]
                if vertexNum == 0:
                    vertexData["color"] = ColorToString(vc_channel.data[iface].color1)
                elif vertexNum == 1:
                    vertexData["color"] = ColorToString(vc_channel.data[iface].color2)
                elif vertexNum == 2:
                    vertexData["color"] = ColorToString(vc_channel.data[iface].color3)
                elif vertexNum == 3:
                    vertexData["color"] = ColorToString(vc_channel.data[iface].color4)

            vertexNum += 1
            vertexDataEle = et.SubElement(vertexesEle, "VertexData")
            for k, v in vertexData.items():
                vertexDataEle.set(k, v)
    indexesEle.text = indexes

    # for vertex in mesh.vertices:
    #     position = Vector3ToString(vertex.co)
    #     vertexData = {}
    #     vertexData["position"] = position
    #     vertexData["normal"] = vertex.normal
    #     ConvertBonesWeigths(vertexData, vertex)
    #     meshData.append(vertexData)
    # # ConvertNormals(vertexesEle, mesh)
    # ConvertColors(meshData, mesh)
    # ConvertUV(meshData, mesh)
    # GenerateVertexes(meshData, meshEle, mesh, materialIndex, vertexesEle)
    # # for data in meshData:
    #    print(data)


def ConvertTexture(materialEle, index, textureSlot):
    if textureSlot is None:
        return
    texture = textureSlot.texture
    if texture.type != 'IMAGE':
        return
    image = texture.image
    textureEle = et.SubElement(materialEle, "Texture" + str(index))
    textureEle.set("uvName", textureSlot.uv_layer)
    textureEle.set("resource", "data/" + image.name)
    if texture.extension == 'REPEAT':
        textureEle.set("wrapMode", "REPEAT")
    elif texture.extend == 'CHECKER':
        textureEle.set("wrapMode", "MIRRORED_REPEAT")
    else:
        textureEle.set("wrapMode", "CLAMP_TO_EDGE")

    if textureSlot.blend_type == 'MIX':
        textureEle.set("blendType", "MIX")
    elif textureSlot.blend_type == 'ADD':
        textureEle.set("blendType", "ADD")
    elif textureSlot.blend_type == 'SUBTRACT':
        textureEle.set("blendType", "SUB")
    elif textureSlot.blend_type == 'MULTIPLY':
        textureEle.set("blendType", "MUL")
    elif textureSlot.blend_type == 'SCREEN':
        textureEle.set("blendType", "SCREEN")
    elif textureSlot.blend_type == 'OVERLAY':
        textureEle.set("blendType", "OVERLAY")
    elif textureSlot.blend_type == 'DIFFERENCE':
        textureEle.set("blendType", "DIFF")
    elif textureSlot.blend_type == 'DIVIDE':
        textureEle.set("blendType", "DIV")
    elif textureSlot.blend_type == 'DARKEN':
        textureEle.set("blendType", "DARK")
    elif textureSlot.blend_type == 'LIGHTEN':
        textureEle.set("blendType", "LIGHT")
    elif textureSlot.blend_type == 'HUE':
        textureEle.set("blendType", "BLEND_HUE")
    elif textureSlot.blend_type == 'SATURATION':
        textureEle.set("blendType", "BLEND_SAT")
    elif textureSlot.blend_type == 'VALUE':
        textureEle.set("blendType", "BLEND_VAL")
    elif textureSlot.blend_type == 'COLOR':
        textureEle.set("blendType", "BLEND_COLOR")
    elif textureSlot.blend_type == 'SOFT_LIGHT':
        textureEle.set("blendType", "LIGHT")
    elif textureSlot.blend_type == 'LINEAR_LIGHT':
        textureEle.set("blendType", "LIGHT")
    else:
        textureEle.set("blendType", "MIX")

    if textureSlot.use_map_alpha:
        textureEle.set("mapType", "ALPHA")
    elif textureSlot.use_map_ambient:
        textureEle.set("mapType", "AMB")
    elif textureSlot.use_map_color_diffuse or textureSlot.use_map_diffuse:
        textureEle.set("mapType", "COL")
    elif textureSlot.use_map_emit:
        textureEle.set("mapType", "EMIT")
    elif textureSlot.use_map_normal:
        textureEle.set("mapType", "NORM")
    elif textureSlot.use_map_color_reflection:
        textureEle.set("mapType", "COLMIR")
    elif textureSlot.use_map_color_spec or textureSlot.use_map_specular:
        textureEle.set("mapType", "SPEC")
    else:
        textureEle.set("mapType", "COL")

    if texture.filter_type == 'BOX':
        textureEle.set("filterMode", "NEAREST")
    else:
        textureEle.set("filterMode", "BILINEAR")

    textureEle.set("flags", "11")
    textureEle.set("flagNames", " INVERT_Y GENERATE_MIPMAPS")


def ConvertMaterial(materialsEle, material):
    print("Converting material " + material.name)
    materialEle = et.SubElement(materialsEle, "Material")
    alpha = 1  # full opaque
    alphaForSpecular = 1  # full opaque
    transparent = material.use_transparency
    if transparent:
        alpha = material.alpha
        alphaForSpecular = material.specular_alpha
    materialEle.set("name", material.name)
    materialEle.set("shadeless", BoolToString(material.use_shadeless))
    materialEle.set("castShadow", BoolToString(material.use_cast_shadows))
    materialEle.set("receiveShadows", BoolToString(material.use_shadows))
    if material.game_settings.use_backface_culling and not transparent:
        materialEle.set("cullFaceMode", "BACK")
    elif material.game_settings.invisible and not transparent:
        materialEle.set("cullFaceMode", "FRONT_AND_BACK")
    else:
        materialEle.set("cullFaceMode", "DISABLED")
    # materialEle.set("friction", ???)
    materialEle.set("ambientIntensity", FloatToString(material.ambient))
    materialEle.set(
        "diffuseIntensity", FloatToString(material.diffuse_intensity))
    materialEle.set(
        "specularIntensity", FloatToString(material.specular_intensity))
    materialEle.set("diffuse", ColorToString(material.diffuse_color))
    materialEle.set("specular", ColorToString(material.specular_color))
    materialEle.set("shininess", str(material.specular_hardness))
    if material.type == 'WIRE':
        materialEle.set("fillMode", "WIREFRAME")
    else:
        materialEle.set("fillMode", "SOLID")
    materialEle.set("alpha", FloatToString(alpha))
    materialEle.set("alphaForSpecular", FloatToString(alphaForSpecular))
    materialEle.set("isTransparent", BoolToString(transparent))
    if material.game_settings.text:
        materialEle.set("renderPass", "TEXT")
    elif material.use_vertex_color_paint:
        materialEle.set("renderPass", "VERTEXCOLOR")
    elif material.use_shadeless:
        materialEle.set("renderPass", "UNLIT")
    else:
        materialEle.set("renderPass", "PERPIXEL")
    # FIX Engine (Load/Save)
    if material.game_settings.face_orientation == 'HALO':
        materialEle.set("billboardType", "SPHERICAL")
    elif material.game_settings.face_orientation == 'BILLBOARD':
        materialEle.set("billboardType", "CYLINDRICAL")
    else:
        materialEle.set("billboardType", "NONE")

    for index, textureSlot in enumerate(material.texture_slots):
        ConvertTexture(materialEle, index, textureSlot)


def ConvertMaterials(appEle):
    materialsEle = et.SubElement(appEle, "Materials")
    for material in bpy.data.objects.data.materials:
        ConvertMaterial(materialsEle, material)


def GetArmatureMeshObj(armatureObj):
    for obj in bpy.data.objects:
        if obj.type == 'MESH' and obj.parent == armatureObj:
            return obj
    return None


def BuildBonetree(parentEle, bone):
    boneEle = et.SubElement(parentEle, "Bone")
    boneEle.set("name", bone.name)

    parBind = mathutils.Matrix()
    if bone.parent:
        parBind = bone.parent.matrix_local.inverted()

    bind = parBind * bone.matrix_local
    loc, rot, sca = bind.decompose()

    boneEle.set("position", Vector3ToString(loc))
    boneEle.set("orientation", QuaternionToString(rot))
    boneEle.set("scale", Vector3ToString(sca))

    for child in bone.children:
        BuildBonetree(boneEle, child)


def ConvertPoseBone(shaderOrderEle, poseBone):
    boneEle = et.SubElement(shaderOrderEle, "Bone")
    boneEle.set("name", poseBone.name)
    boneEle.set("offsetMatrix", Matrix4ToString(poseBone.matrix))


def ConvertArmature(armaturesEle, armatureObj):
    armature = armatureObj.data
    armatureEle = GetChildEle(armaturesEle, "Skeleton", "name", armature.name)
    if armatureEle is None:
        print("Converting armature " + armature.name)
        armatureEle = et.SubElement(armaturesEle, "Skeleton")
        armatureEle.set("name", armature.name)

        meshObj = GetArmatureMeshObj(armatureObj)
        assert meshObj
        shaderOrderEle = et.SubElement(armatureEle, "ShaderOrder")
        for bone in armatureObj.pose.bones:
            ConvertPoseBone(shaderOrderEle, bone)
        armature = armatureObj.data
        BonesEle = et.SubElement(armatureEle, "Bones")
        for bone in armature.bones:
            if not bone.parent:
                BuildBonetree(BonesEle, bone)


def ConvertImage(resourcesEle, image):
    if image.users > 0:
        resourceEle = et.SubElement(resourcesEle, "Resource")
        resourceEle.set("name", "data/" + image.name)


def ConvertResources(appEle):
    resourcesEle = et.SubElement(appEle, "Resources")
    for image in bpy.data.objects.data.images:
        ConvertImage(resourcesEle, image)


def ExtractTransform(obj):
    # if obj.parent:
    #     parentInv = obj.parent.matrix_local.inverted()
    #     m = parentInv * obj.matrix_local
    #     return m.decompose()
    # else:
        # obj.location, obj.rotation_quaternion, obj.scale
    return obj.matrix_local.decompose()


def GetChildEle(ele, childEleName, attName, attNameValue):
    childEle = ele.findall(
        ".//" + childEleName + "[@" + attName + "='" + attNameValue + "']")
    if len(childEle) == 1:
        return childEle[0]
    return None


def GetOrCreateChildEle(ele, childEleName, attName, attNameValue):
    childEle = GetChildEle(ele, childEleName, attName, attNameValue)
    if childEle is None:
        childEle = et.SubElement(ele, childEleName)
        childEle.set(attName, attNameValue)
    return childEle


def GetMesh(name):
    for mesh in bpy.data.objects.data.meshes:
        if mesh.name == name:
            return mesh
    return None


def ConvertMeshObject(meshesEle, parentEle, obj):
    nMaterials = len(obj.material_slots)
    materialIndex = 0
    while nMaterials > materialIndex:
        materialSlot = obj.material_slots[materialIndex]
        meshName = obj.data.name
        if materialIndex == 0:
            position, rotation, scale = ExtractTransform(obj)
            parentEle = sceneNodeEle = CreateSceneNode(
                obj.name, parentEle, obj, position, rotation, scale)
            sceneNodeEle.set("meshName", meshName)
        else:
            newNodeName = obj.name + "_" + materialSlot.name
            sceneNodeEle = CreateSceneNode(
                newNodeName, parentEle, obj, None, None, None)
            newMeshName = meshName + "_" + materialSlot.name
            ConvertMesh(
                newMeshName, meshesEle, GetMesh(meshName), materialIndex)
            sceneNodeEle.set("meshName", newMeshName)
        sceneNodeEle.set("materialName", materialSlot.name)
        materialIndex = materialIndex + 1


def ConvertArmatureObject(armaturesEle, parentEle, armatureObj):
    armature = armatureObj.data
    armatureEle = GetChildEle(armaturesEle, "Skeleton", "name", armature.name)
    if armatureEle is None:
        print("Armature {:s} not found!!!".format(armature.name))
        return

    position, rotation, scale = ExtractTransform(armatureObj)
    sceneNodeEle = CreateSceneNode(
        armatureObj.name, parentEle, armatureObj, position, rotation, scale)
    sceneNodeEle.set("skeleton", armature.name)


def ConvertLampObject(parentEle, obj):
    position, rotation, scale = ExtractTransform(obj)
    sceneNodeEle = CreateSceneNode(
        obj.name, parentEle, obj, position, rotation, scale)
    sceneNodeEle.set("nodeType", "Light")
    light = obj.data
    sceneNodeEle.set("type", LightTypeToString(light.type))
    if light.type == 'SPOT':
        cutoff = math.degrees(light.spot_size)
        sceneNodeEle.set("spotCutOff", FloatToString(cutoff))
    sceneNodeEle.set("energy", FloatToString(light.energy))
    sceneNodeEle.set("color", ColorToString(light.color))
    sceneNodeEle.set("diffuse", BoolToString(light.use_diffuse))
    sceneNodeEle.set("specular", BoolToString(light.use_specular))
    sceneNodeEle.set("distance", FloatToString(light.distance))
    sceneNodeEle.set("shadows", BoolToString(light.cycles.cast_shadow))
    sceneNodeEle.set("shadowColor", ColorToString(light.shadow_color))
    sceneNodeEle.set("onlyShadow", BoolToString(light.use_only_shadow))
    # TODO: CHECK IF THIS IS A GOOD APROXIMATION
    bias = light.shadow_buffer_bias * 0.001
    sceneNodeEle.set("shadowBias", FloatToString(bias))
    sceneNodeEle.set(
        "shadowClipStart", FloatToString(light.shadow_buffer_clip_start))
    sceneNodeEle.set(
        "shadowClipEnd", FloatToString(light.shadow_buffer_clip_end))


def ConvertCameraObject(parentEle, obj):
    position, rotation, scale = ExtractTransform(obj)
    sceneNodeEle = CreateSceneNode(
        obj.name, parentEle, obj, position, rotation, scale)
    sceneNodeEle.set("nodeType", "Camera")
    camera = obj.data
    sceneNodeEle.set("zNear", FloatToString(camera.clip_start))
    sceneNodeEle.set("zFar", FloatToString(camera.clip_end))
    if camera.type == 'ORTHO':
        sceneNodeEle.set("isOrtho", BoolToString(True))
    else:
        sceneNodeEle.set("isOrtho", BoolToString(False))
    sceneNodeEle.set("orthoScale", FloatToString(camera.ortho_scale))
    sceneNodeEle.set("sensorFit", SensorFitToString(camera.sensor_fit))

    fov = 2 * math.atan(0.5 * camera.sensor_width / camera.lens)
    if camera.sensor_fit == 'VERTICAL':
        fov = 2 * math.atan(0.5 * camera.sensor_height / camera.lens)
    sceneNodeEle.set("fovy", FloatToString(fov))


def ConvertObject(armaturesEle, meshesEle, sceneEle, obj, scene):
    type = obj.type
    print("Converting object type " + type)
    parentEle = sceneEle
    if obj.parent:
        parentEle = GetChildEle(sceneEle, "SceneNode", "name", obj.parent.name)

    if type == 'MESH':
        ConvertMeshObject(meshesEle, parentEle, obj)
    elif type == 'ARMATURE':
        ConvertArmatureObject(armaturesEle, parentEle, obj)
    elif type == 'LAMP':
        ConvertLampObject(parentEle, obj)
    elif type == 'CAMERA':
        ConvertCameraObject(parentEle, obj)
    else:
        position, rotation, scale = ExtractTransform(obj)
        CreateSceneNode(obj.name, parentEle, obj, position, rotation, scale)


def GetChannelMask(transform_name):
    if transform_name == "location":
        return 1 << 0
    elif transform_name == "rotation_quaternion":
        return 1 << 1
    elif transform_name == "scale":
        return 1 << 2
    else:
        return 0


def GetPathData(path):
    pathType = path[:10]
    chan_name = path[path.find('"') + 1: path.rfind('"')]
    transform_name = path[path.rfind('.') + 1:]
    return pathType, chan_name, transform_name


def ConvertTransform(keyframeEle, transforms):
    loc = mathutils.Vector((0, 0, 0))
    rot = mathutils.Quaternion()
    rot.identity()
    sca = mathutils.Vector((1, 1, 1))
    for name, indeces in transforms.items():
        for index, value in indeces.items():
            if name == "rotation_quaternion":
                rot[index] = value
            elif name == "location":
                loc[index] = value
            elif name == "scale":
                sca[index] = value
    return loc, rot, sca


def ConverKeyfames(tracksEle, data, convert):
    for nodeName, v0 in data.items():
        trackEle = GetChildEle(tracksEle, "Track", "nodeName", nodeName)
        keyframesEle = et.SubElement(trackEle, "KeyFrames")
        for frame, transforms in v0.items():
            keyframeEle = GetOrCreateChildEle(
                keyframesEle, "KeyFrame", "time",
                FloatToString(convert(frame)))
            loc, rot, sca = ConvertTransform(keyframeEle, transforms)
            keyframeEle.set("position", Vector3ToString(loc))
            keyframeEle.set("rotation", QuaternionToString(rot))
            keyframeEle.set("scale", Vector3ToString(sca))


def SetKeyframeData(data, chan_name, frame, transform_name, index, value):
    data0 = data.setdefault(
        chan_name, {frame: {transform_name: {index: value}}})
    data1 = data0.setdefault(frame, {transform_name: {index: value}})
    data2 = data1.setdefault(transform_name, {index: value})
    data2.setdefault(index, value)


def ConvertCurve(tracksEle, curve, data):
    if len(curve.keyframe_points) == 0:
        return
    curve.update()
    pathType, chan_name, transform_name = GetPathData(curve.data_path)
    index = curve.array_index
    trackEle = GetChildEle(tracksEle, "Track", "nodeName", chan_name)
    channelMask = GetChannelMask(transform_name)
    if trackEle is None:
        trackEle = et.SubElement(tracksEle, "Track")
        trackEle.set("nodeName", chan_name)
    else:
        channelMask |= int(trackEle.get("channelMask"), 2)

    channelMask = bin(channelMask)
    channelMask = channelMask[channelMask.find('b') + 1:]
    trackEle.set("channelMask", channelMask)
    frame0 = int(curve.keyframe_points[0].co[0])
    value0 = curve.keyframe_points[0].co[1]
    SetKeyframeData(
        data, chan_name, frame0, transform_name, index, value0)
    if len(curve.keyframe_points) > 1:
        frame1 = int(curve.keyframe_points[1].co[0])
        value1 = curve.keyframe_points[1].co[1]
        SetKeyframeData(
            data, chan_name, frame1, transform_name, index, value1)


def ConvertAnimation(animationsEle, action, scene):
    fps = scene.render.fps / scene.render.fps_base
    frame_begin, frame_end = action.frame_range[0], action.frame_range[1]
    trackLength = (frame_end - frame_begin) / fps

    def ConvertFrame2Time(frame):
        return (trackLength / (frame_end - frame_begin)) * frame

    animationEle = et.SubElement(animationsEle, "Animation")
    animationEle.set("length", FloatToString(trackLength))
    animationEle.set("name", action.name)
    tracksEle = et.SubElement(animationEle, "Tracks")
    data = {}
    for curve in action.fcurves:
        ConvertCurve(tracksEle, curve, data)
    ConverKeyfames(tracksEle, data, ConvertFrame2Time)


def ConvertMeshes(appEle):
    meshesEle = et.SubElement(appEle, "Meshes")
    for mesh in bpy.data.objects.data.meshes:
        ConvertMesh(mesh.name, meshesEle, mesh, 0)
    return meshesEle


def ConvertSkeletons(appEle):
    armaturesEle = et.SubElement(appEle, "Skeletons")
    for obj in bpy.data.objects:
        if obj.type == 'ARMATURE':
            ConvertArmature(armaturesEle, obj)
    return armaturesEle


def ConvertAnimations(appEle, scene):
    animationsEle = et.SubElement(appEle, "Animations")
    for action in bpy.data.actions:
        ConvertAnimation(animationsEle, action, scene)
    return animationsEle


def ConvertPhysicShape(shapesEle, obj):
    # print(mathutils.Vector(obj.bound_box[0]))
    # print(mathutils.Vector(obj.bound_box[7]))
    name, scaleStr, typeStr = GetPhysicShapeName(obj)
    shapeEle = GetChildEle(shapesEle, "Shape", "name", name)
    if not shapeEle:
        shapeEle = et.SubElement(shapesEle, "Shape")
        shapeEle.set("name", name)
        shapeEle.set("scale", scaleStr)
        shapeEle.set("margin", FloatToString(obj.game.collision_margin))
        shapeEle.set("type", typeStr)


def ConvertPhysicShapes(appEle):
    shapesEle = et.SubElement(appEle, "Shapes")
    for obj in bpy.data.objects:
        if obj.game.physics_type != 'NO_COLLISION':
            ConvertPhysicShape(shapesEle, obj)
    return shapesEle


def ConvertPhysicsScene(sceneEle, scene):
    # print("Converting physics for scene " + scene.name)
    physicsEle = et.SubElement(sceneEle, "Physics")
    gravity = scene.gravity
    gravity.z, gravity.y = gravity.y, gravity.z
    physicsEle.set("gravity", Vector3ToString(gravity))
    physicsEle.set("fps", str(scene.game_settings.fps))
    physicsEle.set("maxSubSteps", str(scene.game_settings.physics_step_max))


def ConvertScene(appEle, scene):

    print("Converting scene " + scene.name)
    meshesEle = ConvertMeshes(appEle)
    armaturesEle = ConvertSkeletons(appEle)
    ConvertAnimations(appEle, scene)
    ConvertPhysicShapes(appEle)

    sceneEle = et.SubElement(appEle, "Scene")
    if scene.camera is not None:
        sceneEle.set("mainCamera", scene.camera.name)
    sceneEle.set("ambient", ColorToString(scene.world.ambient_color))
    sceneEle.set("horizon", ColorToString(scene.world.horizon_color))
    sceneEle.set("enableFog", BoolToString(scene.world.mist_settings.use_mist))
    sceneEle.set("fogMinIntensity", FloatToString(
        scene.world.mist_settings.intensity))
    sceneEle.set("fogStart", FloatToString(scene.world.mist_settings.start))
    sceneEle.set("fogDepth", FloatToString(scene.world.mist_settings.depth))
    sceneEle.set("fogHeight", FloatToString(scene.world.mist_settings.height))

    orientation = mathutils.Quaternion(
        (1, 0, 0), math.radians(-90.0))

    sceneNodeEle = CreateSceneNode(
        scene.name, sceneEle, None, None, orientation, None)

    ConvertPhysicsScene(sceneEle, scene)

    for obj in scene.objects:
        scene.objects.active = obj
        currentMode = obj.mode
        if currentMode != 'OBJECT':
            bpy.ops.object.mode_set(mode='OBJECT')
        ConvertObject(armaturesEle, meshesEle, sceneNodeEle, obj, scene)
        if currentMode != 'OBJECT':
            bpy.ops.object.mode_set(mode=currentMode)


def ConvertScenes(appEle):
    for scene in bpy.data.objects.data.scenes:
        ConvertScene(appEle, scene)


def ConvertApp():
    # export to blend file location
    basedir = os.path.dirname(bpy.data.filepath)
    if not basedir:
        raise Exception("Blend file is not saved")
    filename = os.path.splitext(
        os.path.basename(bpy.data.filepath))[0]
    appEle = et.Element("App")
    ConvertResources(appEle)
    ConvertMaterials(appEle)
    ConvertScenes(appEle)
    tree = et.ElementTree(appEle)
    tree.write(basedir + "/../data/" + filename + ".xml")

ConvertApp()
