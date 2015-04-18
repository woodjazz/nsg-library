/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "AppData.h"
#include "Resource.h"
#include "Texture.h"
#include "Sound.h"
#include "Log.h"
#include "Check.h"
#include "Util.h"
#include "Scene.h"
#include "Path.h"
#include "Material.h"
#include "Mesh.h"
#include "pugixml.hpp"
namespace NSG
{
    AppData::AppData(PResource resource)
    {
        CHECK_CONDITION(resource->IsReady(), __FILE__, __LINE__);
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer((void*)resource->GetData(), resource->GetBytes());
        if (result)
        {
            pugi::xml_node appNode = doc.child("App");
            resources_ = Resource::LoadResources(resource, appNode);
			sounds_ = Sound::LoadSounds(resource, appNode);
            meshes_ = Mesh::LoadMeshes(resource, appNode);
            materials_ = Material::LoadMaterials(resource, appNode);
            pugi::xml_node child = appNode.child("Scene");
            while (child)
            {
                auto scene = std::make_shared<Scene>("scene");
                scene->Load(child);
                scenes_.push_back(scene);
                child = child.next_sibling("Scene");
            }
        }
        else
        {
            TRACE_LOG("XML parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]");
            TRACE_LOG("Error description: " << result.description());
            TRACE_LOG("Error offset: " << result.offset << " (error at [..." << (result.offset) << "]");
            CHECK_ASSERT(false, __FILE__, __LINE__);
        }
    }
}