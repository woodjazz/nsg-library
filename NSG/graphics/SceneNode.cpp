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
#include "SceneNode.h"
#include "FrameColorSelection.h"
#include "App.h"
#include "Check.h"
#include "Behavior.h"
#include "Technique.h"
#include "Graphics.h"
#include "Material.h"
#include "ModelMesh.h"
#include "Scene.h"
#include "Light.h"
#include "Camera.h"
#include "Octree.h"
#include "Util.h"
#include "pugixml.hpp"

namespace NSG
{
    SceneNode::SceneNode(const std::string& name, Scene* scene)
        : Node(name),
          octant_(nullptr),
          occludee_(false),
          worldBBNeedsUpdate_(true),
          meshIndex_(-1),
          materialIndex_(-1),
          scene_(scene),
          app_(*App::this_)
    {
        CHECK_ASSERT(scene, __FILE__, __LINE__);
    }

    SceneNode::SceneNode(PResource resource, const std::string& name, Scene* scene)
        : Node(name),
          app_(*App::this_),
          octant_(nullptr),
          occludee_(false),
          worldBBNeedsUpdate_(true),
          resource_(resource),
          meshIndex_(-1),
          materialIndex_(-1),
          scene_(scene)
    {
        CHECK_ASSERT(scene, __FILE__, __LINE__);
    }

    SceneNode::~SceneNode()
    {
        if(scene_->GetOctree())
            scene_->GetOctree()->Remove(this);
        Context::RemoveObject(this);
    }

    bool SceneNode::IsValid()
    {
        if (resource_)
            return resource_->IsLoaded();
        else
            return true;
    }

    void SceneNode::AllocateResources()
    {
        if (resource_)
        {
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_buffer_inplace((void*)resource_->GetData(), resource_->GetBytes());
            if (result)
            {
                CachedData data;
                LoadMeshesAndMaterials(doc, data);
                Load(doc, data);
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

    PSceneNode SceneNode::CreateChild(const std::string& name)
    {
        PSceneNode obj(new SceneNode(name, scene_));
        AddChild(obj);
        return obj;
    }

    void SceneNode::Set(PMaterial material)
    {
        material_ = material;
    }

    void SceneNode::Set(PMesh mesh)
    {
        if (mesh != mesh_)
        {
            mesh_ = mesh;
            worldBB_ = BoundingBox();

            if (!mesh)
            {
                occludee_ = false;
                scene_->GetOctree()->Remove(this);
            }
            else
            {
                occludee_ = true;
                worldBBNeedsUpdate_ = true;
                scene_->GetOctree()->InsertUpdate(this);
            }
        }
    }

    void SceneNode::AddBehavior(PBehavior behavior)
    {
        if (behavior)
        {
            behaviors_.push_back(behavior);
            behavior->sceneNode_ = this;
        }
    }

    void SceneNode::Start()
    {
        for(auto& obj: behaviors_)
            obj->Start();

		for (auto& obj : children_)
			obj->Start();
    }

    void SceneNode::Update()
    {
        for(auto& obj: behaviors_)
            obj->Update();

		for (auto& obj : children_)
			obj->Update();
    }

    void SceneNode::ViewChanged(int width, int height)
    {
        for (auto& obj : behaviors_)
            obj->ViewChanged(width, height);

		for (auto& obj : children_)
			obj->ViewChanged(width, height);
	}

    void SceneNode::OnMouseMove(float x, float y)
    {
        for (auto& obj : behaviors_)
            obj->OnMouseMove(x, y);

		for (auto& obj : children_)
			obj->OnMouseMove(x, y);
    }

    void SceneNode::OnMouseDown(float x, float y)
    {
        for (auto& obj : behaviors_)
            obj->OnMouseDown(x, y);

		for (auto& obj : children_)
			obj->OnMouseDown(x, y);
    }

    void SceneNode::OnMouseWheel(float x, float y)
    {
        for (auto& obj : behaviors_)
            obj->OnMouseWheel(x, y);
	
		for (auto& obj : children_)
			obj->OnMouseWheel(x, y);
	}

    void SceneNode::OnMouseUp(float x, float y)
    {
        for (auto& obj : behaviors_)
            obj->OnMouseUp(x, y);

		for (auto& obj : children_)
			obj->OnMouseUp(x, y);
	}

    void SceneNode::OnKey(int key, int action, int modifier)
    {
        for (auto& obj : behaviors_)
            obj->OnKey(key, action, modifier);
	
		for (auto& obj : children_)
			obj->OnKey(key, action, modifier);
	}

    void SceneNode::OnChar(unsigned int character)
    {
        for (auto& obj : behaviors_)
            obj->OnChar(character);
	
		for (auto& obj : children_)
			obj->OnChar(character);
	}

    void SceneNode::OnDirty() const
    {
        worldBBNeedsUpdate_ = true;

        if (octant_)
            scene_->NeedUpdate((SceneNode*)this);
    }

    const BoundingBox& SceneNode::GetWorldBoundingBox() const
    {
        if (worldBBNeedsUpdate_)
        {
            if (mesh_ && mesh_->IsReady())
            {
                worldBB_ = mesh_->GetBB();
                worldBB_.Transform(*this);
                worldBBNeedsUpdate_ = false;
            }
        }
        return worldBB_;
    }

    void SceneNode::Save(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("SceneNode");

        {
            std::stringstream ss;
            ss << GetName();
            child.append_attribute("name") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetPosition();
            child.append_attribute("position") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetOrientation();
            child.append_attribute("orientation") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetScale();
            child.append_attribute("scale") = ss.str().c_str();
        }

        if (materialIndex_ != -1)
        {
            std::stringstream ss;
            ss << materialIndex_;
            child.append_attribute("materialIndex") = ss.str().c_str();
        }

        if (meshIndex_ != -1)
        {
            std::stringstream ss;
            ss << meshIndex_;
            child.append_attribute("meshIndex") = ss.str().c_str();
        }

        for (auto& obj : children_)
            obj->Save(child);
    }

    void SceneNode::Load(const pugi::xml_document& doc, const CachedData& data)
    {
        std::stringstream query;
        query << "/Scene/SceneNode[@name ='" << name_ << "']";
        pugi::xpath_node xpathNode = doc.select_single_node(query.str().c_str());
        pugi::xml_node child = xpathNode.node();
        LoadNode(child, data);
    }

    void SceneNode::LoadNode(const pugi::xml_node& node, const CachedData& data)
    {
        Vertex3 position = GetVertex3(node.attribute("position").as_string());
        SetPosition(position);

        Quaternion orientation = GetQuaternion(node.attribute("orientation").as_string());
        SetOrientation(orientation);

        Vertex3 scale = GetVertex3(node.attribute("scale").as_string());
        SetScale(scale);

        pugi::xml_attribute attribute = node.attribute("materialIndex");
        if (attribute)
        {
            materialIndex_ = attribute.as_int();
            Set(data.materials_.at(materialIndex_));
        }

        attribute = node.attribute("meshIndex");
        if (attribute)
        {
            meshIndex_ = attribute.as_int();
            Set(data.meshes_.at(meshIndex_));
        }

		{
			pugi::xml_node child = node.child("Light");
			if (child)
			{
				PLight obj = scene_->CreateLight(child.attribute("name").as_string());
				obj->Load(child);
				AddChild(obj);
			}
		}

		{
			pugi::xml_node child = node.child("Camera");
			if (child)
			{
				PCamera obj = scene_->CreateCamera(child.attribute("name").as_string());
				obj->Load(child);
				AddChild(obj);
			}
		}


        for (pugi::xml_node child = node.child("SceneNode"); child; child = child.next_sibling("SceneNode"))
        {
			PSceneNode childNode = CreateChild(child.attribute("name").as_string());
            childNode->LoadNode(child, data);
        }
    }

    void SceneNode::LoadMeshesAndMaterials(const pugi::xml_document& doc, CachedData& data)
    {
        {
            std::stringstream query;
            query << "/Scene/Meshes/Mesh";
            pugi::xpath_node xpathNode = doc.select_single_node(query.str().c_str());
            pugi::xml_node child = xpathNode.node();
            while (child)
            {
                PMesh mesh(app_.CreateModelMesh());
                data.meshes_.push_back(mesh);
                mesh->Load(child);
                child = child.next_sibling("Mesh");
            }
        }

        {
            std::stringstream query;
            query << "/Scene/Materials/Material";
            pugi::xpath_node xpathNode = doc.select_single_node(query.str().c_str());
            pugi::xml_node child = xpathNode.node();
            while (child)
            {
                PMaterial material(app_.CreateMaterial());
                data.materials_.push_back(material);
                material->Load(child);
                child = child.next_sibling("Material");
            }
        }
    }
}
