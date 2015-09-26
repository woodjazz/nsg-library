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
#include "LoaderApp.h"
#include "Engine.h"
#include "Check.h"
#include "Resource.h"
#include "LoaderXML.h"
#include "Texture.h"
#include "Sound.h"
#include "Log.h"
#include "Check.h"
#include "Util.h"
#include "Scene.h"
#include "Path.h"
#include "Material.h"
#include "Shape.h"
#include "ModelMesh.h"
#include "Program.h"
#include "Skeleton.h"
#include "Animation.h"
#include "pugixml.hpp"

namespace NSG
{
    LoaderApp::LoaderApp(PResource resource)
        : resource_(resource),
          loaded_(false),
          signalLoaded_(new SignalEmpty)
    {
    }

    LoaderApp::~LoaderApp()
    {
    }
    
    static bool AreReady()
    {
        return Resource::AreReady() &&
        Sound::AreReady() &&
        Mesh::AreReady() &&
        Material::AreReady() &&
        Shape::AreReady() &&
        Skeleton::AreReady() &&
        Animation::AreReady() &&
        LoaderXML::AreReady();
    }

    SignalEmpty::PSignal LoaderApp::Load()
    {
        slotUpdate_ = Engine::SigUpdate()->Connect([this](float deltaTime)
        {
            if (!loaded_ && resource_->IsReady())
            {
                auto loader = LoaderXML::GetOrCreate(resource_->GetName());
                loader->Set(resource_);
                CHECK_CONDITION(loader->IsReady());
                resources_ = Object::LoadAll<Resource, Resource>(loader, "Resources");
                sounds_ = Object::LoadAll<Sound, Sound>(loader, "Sounds");
                meshes_ = Object::LoadAll<Mesh, ModelMesh>(loader, "Meshes");
                materials_ = Object::LoadAll<Material, Material>(loader, "Materials");
                shapes_ = Object::LoadAll<Shape, Shape>(loader, "Shapes");
                skeletons_ = Object::LoadAll<Skeleton, Skeleton>(loader, "Skeletons");
                animations_ = Object::LoadAll<Animation, Animation>(loader, "Animations");

                auto& doc = loader->GetDocument();
                auto appNode = doc.child("App");
                pugi::xml_node child = appNode.child("Scene");
                while (child)
                {
                    std::string sceneName = child.attribute("name").as_string();
                    auto scene = std::make_shared<Scene>(sceneName);
                    scene->Load(child);
                    scenes_.push_back(scene);
                    child = child.next_sibling("Scene");
                }
                loader->Invalidate(); // free mem
                loaded_ = true;
            }
            else if (AreReady())
            {
                signalLoaded_->Run();
                slotUpdate_ = nullptr;
            }
        });

        return signalLoaded_;
    }

    PScene LoaderApp::GetScene(int idx) const
    {
        CHECK_ASSERT(idx < scenes_.size());
        return scenes_[idx];
    }
}