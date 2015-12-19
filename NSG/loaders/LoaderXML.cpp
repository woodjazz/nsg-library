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
#include "LoaderXML.h"
#include "Check.h"
#include "ResourceFile.h"
#include "Sound.h"
#include "ModelMesh.h"
#include "Material.h"
#include "Shape.h"
#include "Skeleton.h"
#include "Animation.h"
#include "Engine.h"
#include "Scene.h"
#include <iterator>

namespace NSG
{
    template<> std::map<std::string, PLoaderXML> StrongFactory<std::string, LoaderXML>::objsMap_ = std::map<std::string, PLoaderXML> {};

    LoaderXML::LoaderXML(const std::string& name)
        : Object(name),
          loaded_(false),
          signalLoaded_(new SignalEmpty),
          signalProgress_(new SignalFloat)
    {
    }

    LoaderXML::~LoaderXML()
    {
    }

    void LoaderXML::Set(PResource resource)
    {
        if (resource != resource_)
        {
            resource_ = resource;
            loaded_ = false;
            Invalidate();
        }
    }

    bool LoaderXML::IsValid()
    {
        return resource_ && resource_->IsReady();
    }

    void LoaderXML::AllocateResources()
    {
        pugi::xml_parse_result result = doc_.load_buffer((void*)resource_->GetData(), resource_->GetBytes());
        if (!result)
        {
            LOGE("Cannot load XML %s. Error description: %s", resource_->GetName().c_str(), result.description());
        }
    }

    void LoaderXML::ReleaseResources()
    {
		loaded_ = false;
    }

    pugi::xml_node LoaderXML::GetNode(const std::string& type, const std::string& name) const
    {
        auto appNode = doc_.child("App");
        auto collection = appNode.child(type.c_str());
        auto node = collection.find_child([&](pugi::xml_node & node) { return name == node.attribute("name").as_string(); });
        return node;
    }

    bool LoaderXML::AreReady()
    {
        auto total = objects_.size();
		if (total)
		{
			auto obj = *objects_.begin();
			if (obj->IsReady())
				objects_.erase(obj);
			SigProgress()->Run(100.f / total);
			return false;
		}
		return true;
    }

	void LoaderXML::Load()
	{
		if (!loaded_ && IsReady())
		{
			resources_ = Object::LoadAll<Resource, ResourceFile>(this, "Resources");
			objects_.insert(resources_.begin(), resources_.end());
			sounds_ = Object::LoadAll<Sound, Sound>(this, "Sounds");
			objects_.insert(sounds_.begin(), sounds_.end());
			meshes_ = Object::LoadAll<Mesh, ModelMesh>(this, "Meshes");
			objects_.insert(meshes_.begin(), meshes_.end());
			materials_ = Object::LoadAll<Material, Material>(this, "Materials");
			objects_.insert(materials_.begin(), materials_.end());
			shapes_ = Object::LoadAll<Shape, Shape>(this, "Shapes");
			objects_.insert(shapes_.begin(), shapes_.end());
			skeletons_ = Object::LoadAll<Skeleton, Skeleton>(this, "Skeletons");
			objects_.insert(skeletons_.begin(), skeletons_.end());
			animations_ = Object::LoadAll<Animation, Animation>(this, "Animations");
			objects_.insert(animations_.begin(), animations_.end());
			auto appNode = doc_.child("App");
			if (appNode)
			{
				pugi::xml_node child = appNode.child("Scene");
				while (child)
				{
					std::string sceneName = child.attribute("name").as_string();
					auto scene = std::make_shared<Scene>(sceneName);
					scene->Load(child);
					scenes_.push_back(scene);
					child = child.next_sibling("Scene");
				}
			}
			loaded_ = true;
		}
		else if (AreReady())
		{
			slotUpdate_ = nullptr;
			signalLoaded_->Run();
			doc_.reset(); // free mem
		}
	}

    SignalEmpty::PSignal LoaderXML::Load(PResource resource)
    {
        Set(resource);
		slotUpdate_ = Engine::SigUpdate()->Connect([this](float deltaTime)
		{
			Load();
		});
        return signalLoaded_;
    }

    PScene LoaderXML::GetScene(int idx) const
    {
        CHECK_ASSERT(idx < scenes_.size());
        return scenes_[idx];
    }
}