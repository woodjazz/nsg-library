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
#include <sstream>

namespace NSG
{
    SceneNode::SceneNode(const std::string& name)
        : Node(name),
          app_(*App::this_),
          octant_(nullptr),
          occludee_(false),
          worldBBNeedsUpdate_(true),
          serializable_(true),
          signalCollision_(new Signal<const ContactPoint&>())
    {
    }

    SceneNode::~SceneNode()
    {
        Invalidate();
    }

    void SceneNode::Load(PResource resource)
    {
        CHECK_CONDITION(resource->IsReady(), __FILE__, __LINE__);
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer_inplace((void*)resource->GetData(), resource->GetBytes());
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
        resource->Invalidate();
    }

    bool SceneNode::IsValid()
    {
        bool valid = !mesh_ || mesh_->IsReady();
        valid &= !material_ || material_->IsReady();
        valid &= !rigidBody_ || rigidBody_->IsReady();
        return valid;
    }

    void SceneNode::SetMaterial(PMaterial material)
    {
        if (material_ != material)
        {
            material_ = material;
            Invalidate();
        }
    }

    void SceneNode::SetMesh(PMesh mesh)
    {
        if (mesh != mesh_)
        {
            if (mesh_)
                mesh_->RemoveSceneNode(this);

            mesh_ = mesh;
            worldBB_ = BoundingBox();

            if (!mesh)
            {
                occludee_ = false;
                scene_.lock()->GetOctree()->Remove(this);
            }
            else
            {
                mesh->AddSceneNode(this);
                occludee_ = true;
                worldBBNeedsUpdate_ = true;
                scene_.lock()->GetOctree()->InsertUpdate(this);
            }
            Invalidate();
        }
    }

    PRigidBody SceneNode::GetOrCreateRigidBody()
    {
        if (!rigidBody_)
        {
            rigidBody_ = std::make_shared<RigidBody>(std::dynamic_pointer_cast<SceneNode>(shared_from_this()));
            Invalidate();
        }
        return rigidBody_;
    }

    void SceneNode::OnScaleChange()
    {
        if (rigidBody_)
            rigidBody_->ReScale();
    }

    void SceneNode::OnEnable()
    {
        if (mesh_)
        {
            scene_.lock()->GetOctree()->InsertUpdate(this);
        }
    }

    void SceneNode::OnDisable()
    {
        if (mesh_)
        {
            scene_.lock()->GetOctree()->Remove(this);
        }
    }

    void SceneNode::OnDirty() const
    {
        worldBBNeedsUpdate_ = true;
        if (octant_)
            scene_.lock()->NeedUpdate((SceneNode*)this);
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

        {
            std::stringstream ss;
            ss << GetName();
            node.append_attribute("name") = ss.str().c_str();
        }

        node.append_attribute("nodeType") = "SceneNode";

        {
            std::stringstream ss;
            ss << GetPosition();
            node.append_attribute("position") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetOrientation();
            node.append_attribute("orientation") = ss.str().c_str();
        }

        {
            std::stringstream ss;
            ss << GetScale();
            node.append_attribute("scale") = ss.str().c_str();
        }

        if (material_)
        {
            int materialIndex = App::this_->GetMaterialSerializableIndex(material_);
            if (materialIndex != -1)
            {
                std::stringstream ss;
                ss << materialIndex;
                node.append_attribute("materialIndex") = ss.str().c_str();
            }
        }

        if (mesh_)
        {
            int meshIndex = App::this_->GetMeshSerializableIndex(mesh_);
            if (meshIndex != -1)
            {
                std::stringstream ss;
                ss << meshIndex;
                node.append_attribute("meshIndex") = ss.str().c_str();
            }
        }

        SaveChildren(node);
    }

    void SceneNode::SaveChildren(pugi::xml_node& node)
    {
        for (auto& obj : children_)
        {
            pugi::xml_node child = node.append_child("SceneNode");
            obj->Save(child);
        }
    }

    void SceneNode::Load(const pugi::xml_document& doc, const CachedData& data)
    {
        std::stringstream query;
        query << "/Scene/SceneNode[@name ='" << name_ << "']";
        pugi::xpath_node xpathNode = doc.select_single_node(query.str().c_str());
        pugi::xml_node child = xpathNode.node();
        CHECK_ASSERT(child, __FILE__, __LINE__);
        Load(child, data);
    }

    void SceneNode::Load(const pugi::xml_node& node, const CachedData& data)
    {
        name_ = node.attribute("name").as_string();

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
            SetMaterial(data.materials_.at(materialIndex_));
        }

        attribute = node.attribute("meshIndex");
        if (attribute)
        {
            int meshIndex = attribute.as_int();
            SetMesh(data.meshes_.at(meshIndex));
        }

        LoadChildren(node, data);
    }

    void SceneNode::LoadChildren(const pugi::xml_node& node, const CachedData& data)
    {
        pugi::xml_node child = node.child("SceneNode");
        while (child)
        {
            std::string childName = child.attribute("name").as_string();
            if (childName.empty())
                break;
            std::string nodeType = child.attribute("nodeType").as_string();
            CHECK_ASSERT(!nodeType.empty(), __FILE__, __LINE__);
            if (nodeType == "Light")
            {
                Node::CreateChild<Light>(childName, child, data);
            }
            else if (nodeType == "Camera")
            {
                PCamera childNode = Node::GetOrCreateChild<Camera>(childName);
                childNode->Load(child, data);
            }
            else
            {
                CHECK_ASSERT(nodeType == "SceneNode", __FILE__, __LINE__);
                PSceneNode childNode = Node::GetOrCreateChild<SceneNode>(childName);
                childNode->Load(child, data);
            }

            child = child.next_sibling("SceneNode");
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
                PModelMesh mesh(app_.GetOrCreateModelMesh(child.attribute("name").as_string()));
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
                PMaterial material(app_.GetOrCreateMaterial(child.attribute("name").as_string()));
                data.materials_.push_back(material);
                material->Load(child);
                child = child.next_sibling("Material");
            }
        }
    }

    void SceneNode::OnCollision(const ContactPoint& contactInfo)
    {
        signalCollision_->Run(contactInfo);
    }

    void SceneNode::Render()
    {
        if(IsReady())
        {
            Graphics::this_->SetScene(GetScene().get());
            Graphics::this_->Set(material_.get());
            Graphics::this_->SetNode(this);
            Graphics::this_->Set(mesh_.get());
            if (material_)
            {
                auto technique = material_->GetTechnique();
                if (technique)
                    technique->Render(nullptr);
            }

            for (auto& obj : children_)
                std::dynamic_pointer_cast<SceneNode>(obj)->Render();
        }
    }
}
