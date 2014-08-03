/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

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
#include "TimedTask.h"
#include "QueuedTask.h"
#include "FSM.h"
#include "Types.h"
#include "SceneNode.h"
#include "App.h"
#include "Log.h"
#include "Util.h"
#include "Constants.h"
#include "Context.h"

#include "Check.h"
#include "Main.h"
#include "SharedPointers.h"
#include "FrameColorSelection.h"
#include "Camera.h"
#include "Behavior.h"
#include "BoxMesh.h"
#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "Camera.h"
#include "TextMesh.h"
#include "Texture.h"
#include "TextureFile.h"
#include "TextureMemory.h"
#include "Light.h"
#include "IMGUI.h"
#include "IMGUISkin.h"
#include "IMGUIStyle.h"
#include "IMGUIContext.h"
#include "IMGUITitle.h"
#include "Render2Texture.h"
#include "Filter.h"
#include "FilterBlur.h"
#include "FilterBlend.h"
#include "RectangleMesh.h"
#include "ShowTexture.h"
#include "ResourceFile.h"
#include "ResourceMemory.h"
#include "Technique.h"
#include "Pass.h"
#include "Pass2Texture.h"
#include "PassFilter.h"
#include "ProgramPerVertex.h"
#include "ProgramUnlit.h"
#include "ProgramWhiteColor.h"
#include "ProgramSimpleColor.h"
#include "AppConfiguration.h"
#include "AppStatistics.h"
#include "Sound.h"
#include "Music.h"
#include "BoundingBox.h"
#include "Frustum.h"


