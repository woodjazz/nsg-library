/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#pragma once
#include "Animation.h"
#include "AnimationController.h"
#include "AnimationState.h"
#include "AppConfiguration.h"
#include "Batch.h"
#include "Bone.h"
#include "BoundingBox.h"
#include "BoxMesh.h"
#include "Camera.h"
#include "CameraControl.h"
#include "Character.h"
#include "Check.h"
#include "CircleMesh.h"
#include "Color.h"
#include "CylinderMesh.h"
#include "DebugRenderer.h"
#include "EllipseMesh.h"
#include "Engine.h"
#include "FSM.h"
#include "FileSystem.h"
#include "FollowCamera.h"
#include "FontTTFAtlas.h"
#include "FontXMLAtlas.h"
#include "FrameBuffer.h"
#include "Frustum.h"
#include "GUI.h"
#include "HTTPRequest.h"
#include "ICollision.h"
#include "IcoSphereMesh.h"
#include "Image.h"
#include "Keys.h"
#include "Light.h"
#include "LoaderXML.h"
#include "Log.h"
#include "Main.h"
#include "Material.h"
#include "Maths.h"
#include "MemoryManager.h"
#include "MemoryTest.h"
#include "ModelMesh.h"
#include "ParticleSystem.h"
#include "Pass.h"
#include "Path.h"
#include "PhysicsWorld.h"
#include "PlaneMesh.h"
#include "PlayerControl.h"
#include "PointOnSphere.h"
#include "Program.h"
#include "QuadMesh.h"
#include "QueuedTask.h"
#include "Ray.h"
#include "RectangleMesh.h"
#include "Renderer.h"
#include "RenderingCapabilities.h"
#include "RenderingContext.h"
#include "ResourceFile.h"
#include "ResourceFile.h"
#include "RigidBody.h"
#include "RoundedRectangleMesh.h"
#include "Scene.h"
#include "SceneNode.h"
#include "ShadowCamera.h"
#include "ShadowMapDebug.h"
#include "Shape.h"
#include "SharedFromPointer.h"
#include "SharedPointers.h"
#include "Skeleton.h"
#include "SphereMesh.h"
#include "StringConverter.h"
#include "TextMesh.h"
#include "Texture2D.h"
#include "TimedTask.h"
#include "TriangleMesh.h"
#include "Types.h"
#include "Util.h"
#include "Window.h"
#include "imgui.h"
#include "pugixml.hpp"
