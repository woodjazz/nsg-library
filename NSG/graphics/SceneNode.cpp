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
#include "RigidBody.h"
#include "pugixml.hpp"
#include <thread>

namespace NSG
{
    SceneNode::SceneNode(const std::string& name)
        : Node(name),
		app_(*App::this_),
          octant_(nullptr),
          occludee_(false),
          worldBBNeedsUpdate_(true),
          serializable_(true)
    {
    }

    SceneNode::~SceneNode()
    {
        if(rigidBody_)
            rigidBody_->SetSceneNode(nullptr);

		PScene scene = app_.GetCurrentScene();
		if (scene && scene->GetOctree())
			GetScene()->GetOctree()->Remove(this);
        Context::RemoveObject(this);
    }

    void SceneNode::SetResource(PResource resource)
    {
        if (resource_ != resource)
        {
            resource_ = resource;
            Invalidate();
        }
    }

    bool SceneNode::IsValid()
    {
        bool valid = !resource_ || resource_->IsLoaded();
        valid &= !mesh_ || mesh_->IsReady();
        return valid;
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
        PSceneNode obj(new SceneNode(name));
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
				GetScene()->GetOctree()->Remove(this);
            }
            else
            {
                occludee_ = true;
                worldBBNeedsUpdate_ = true;
				GetScene()->GetOctree()->InsertUpdate(this);
            }
        }
    }

    void SceneNode::Set(PRigidBody rigidBody)
    {
        rigidBody_ = rigidBody;
        rigidBody_->SetSceneNode(this);
    }

    void SceneNode::OnScaleChange()
    {
        if(rigidBody_)
            rigidBody_->ReScale();
    }

    void SceneNode::OnEnable()
    {
        if (mesh_)
        {
			GetScene()->GetOctree()->InsertUpdate(this);
        }
    }

    void SceneNode::OnDisable()
    {
        if (mesh_)
        {
			GetScene()->GetOctree()->Remove(this);
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
        for (auto& obj : behaviors_)
            obj->Start();

        for (auto& obj : children_)
            obj->Start();
    }

    void SceneNode::Update()
    {
        if(rigidBody_)
            rigidBody_->IsReady(); // forces the rigidbody to allocate the resources when becomes valid

        for (auto& obj : behaviors_)
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

    void SceneNode::OnMouseDown(int button, float x, float y)
    {
        for (auto& obj : behaviors_)
            obj->OnMouseDown(button, x, y);

        for (auto& obj : children_)
            obj->OnMouseDown(button, x, y);
    }

    void SceneNode::OnMouseWheel(float x, float y)
    {
        for (auto& obj : behaviors_)
            obj->OnMouseWheel(x, y);

        for (auto& obj : children_)
            obj->OnMouseWheel(x, y);
    }

    void SceneNode::OnMouseUp(int button, float x, float y)
    {
        for (auto& obj : behaviors_)
            obj->OnMouseUp(button, x, y);

        for (auto& obj : children_)
            obj->OnMouseUp(button, x, y);
    }

    void SceneNode::OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
    {
        for (auto& obj : behaviors_)
            obj->OnMultiGesture(timestamp, x, y, dTheta, dDist, numFingers);

        for (auto& obj : children_)
            obj->OnMultiGesture(timestamp, x, y, dTheta, dDist, numFingers);
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
		{
			GetScene()->NeedUpdate((SceneNode*)this);
		}
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

    BoundingBox SceneNode::GetWorldBoundingBoxBut(const SceneNode* node) const
    {
        if (node == this)
            return BoundingBox();

        BoundingBox bb(GetWorldBoundingBox());

        for (auto& obj : children_)
            bb.Merge(dynamic_cast<SceneNode*>(obj.get())->GetWorldBoundingBoxBut(node));

        return bb;
    }

	void SceneNode::GetMaterials(std::vector<PMaterial>& materials) const
	{
		materials.push_back(material_);
		for (auto& obj : children_)
			dynamic_cast<SceneNode*>(obj.get())->GetMaterials(materials);
	}

    void SceneNode::Save(pugi::xml_node& node)
    {
        if (!IsSerializable())
            return;

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

        if (material_)
        {
            int materialIndex = App::this_->GetMaterialSerializableIndex(material_);
            if(materialIndex != -1)
            {
                std::stringstream ss;
                ss << materialIndex;
                child.append_attribute("materialIndex") = ss.str().c_str();
            }
        }

        if (mesh_)
        {
            int meshIndex = App::this_->GetMeshSerializableIndex(mesh_);
            if(meshIndex != -1)
            {
                std::stringstream ss;
                ss << meshIndex;
                child.append_attribute("meshIndex") = ss.str().c_str();
            }
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
        CHECK_ASSERT(child, __FILE__, __LINE__);
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
            int materialIndex_ = attribute.as_int();
			Set(data.materials_.at(materialIndex_));
        }

        attribute = node.attribute("meshIndex");
        if (attribute)
        {
            int meshIndex = attribute.as_int();
            Set(data.meshes_.at(meshIndex));
        }

        {
            pugi::xml_node child = node.child("Light");
            if (child)
            {
				PLight obj = GetScene()->CreateLight(child.attribute("name").as_string());
                obj->Load(child);
                AddChild(obj);
            }
        }

        {
            pugi::xml_node child = node.child("Camera");
            if (child)
            {
				PCamera obj = GetScene()->CreateCamera(child.attribute("name").as_string());
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

    void SceneNode::SetDiffuseMap(PTexture texture, bool recursive)
    {
        if(recursive)
        {
            std::vector<PMaterial> materials;
            GetMaterials(materials);
			for (auto& material : materials)
				if (material)
					material->SetDiffuseMap(texture);
        }
		else if(material_)
        {
			material_->SetDiffuseMap(texture);
        }
    }

    void SceneNode::SetNormalMap(PTexture texture, bool recursive)
    {
        if(recursive)
        {
            std::vector<PMaterial> materials;
            GetMaterials(materials);
            for(auto& material: materials)
				if (material)
					material->SetNormalMap(texture);
        }
		else if(material_)
        {
            material_->SetNormalMap(texture);
        }
    }

    void SceneNode::SetLightMap(PTexture texture, bool recursive)
    {
        if(recursive)
        {
            std::vector<PMaterial> materials;
            GetMaterials(materials);
            for(auto& material: materials)
				if (material)
					material->SetLightMap(texture);
        }
		else if(material_)
        {
            material_->SetLightMap(texture);
        }
    }

	PScene SceneNode::GetScene() const
	{
		PScene scene = app_.GetCurrentScene();
		CHECK_ASSERT(scene, __FILE__, __LINE__);
		return scene;
	}

    void SceneNode::OnCollision(const ContactPoint& contactInfo)
    {
        for (auto& obj : behaviors_)
            obj->OnCollision(contactInfo);

        for (auto& obj : children_)
            obj->OnCollision(contactInfo);
    }
}
