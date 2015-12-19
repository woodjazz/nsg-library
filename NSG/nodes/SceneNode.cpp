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
#include "SceneNode.h"
#include "Check.h"
#include "Skeleton.h"
#include "Graphics.h"
#include "Material.h"
#include "ModelMesh.h"
#include "Octree.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Util.h"
#include "RigidBody.h"
#include "Character.h"
#include "AnimationController.h"
#include "StringConverter.h"
#include "SharedFromPointer.h"
#include "pugixml.hpp"
#include <thread>
#include <sstream>
#include <string>

namespace NSG
{
    SceneNode::SceneNode(const std::string& name)
        : Node(name),
          octant_(nullptr),
          worldBBNeedsUpdate_(true),
          serializable_(true),
          signalMeshSet_(new SignalEmpty()),
          signalMaterialSet_(new SignalEmpty()),
          signalCollision_(new Signal<const ContactPoint & >())
    {
        flags_ = (int)SceneNodeFlag::ALLOW_RAY_QUERY;
    }

    SceneNode::~SceneNode()
    {
        if (mesh_)
            mesh_->RemoveSceneNode(this);

        auto scene = GetScene();
        if (scene)
            scene->RemoveFromOctree(this);
    }

    bool SceneNode::CanBeVisible() const
    {
        return mesh_ != nullptr;
    }

    void SceneNode::SetMaterial(PMaterial material)
    {
        if (material_ != material)
        {
            material_ = material;
            signalMaterialSet_->Run();
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

            if (!mesh || IsHidden())
            {
                auto scene = GetScene();
                if (scene)
                    scene->RemoveFromOctree(this);
            }
            else
            {
                mesh->AddSceneNode(this);
                worldBBNeedsUpdate_ = true;
                auto scene = GetScene();
                if (scene)
                    scene->UpdateOctree(this);
            }

            signalMeshSet_->Run();
        }
    }

    PRigidBody SceneNode::GetOrCreateRigidBody()
    {
        CHECK_ASSERT(character_ == nullptr);
        if (!rigidBody_)
			rigidBody_ = std::make_shared<RigidBody>(std::dynamic_pointer_cast<SceneNode>(SharedFromPointerNode(this)));
        return rigidBody_;
    }

    PCharacter SceneNode::GetOrCreateCharacter()
    {
        CHECK_ASSERT(rigidBody_ == nullptr);
        if (!character_)
			character_ = std::make_shared<Character>(std::dynamic_pointer_cast<SceneNode>(SharedFromPointerNode(this)));
        return character_;
    }

    PAnimationController SceneNode::GetOrCreateAnimationController()
    {
        if (!animationController_)
			animationController_ = std::make_shared<AnimationController>(std::dynamic_pointer_cast<SceneNode>(SharedFromPointerNode(this)));
        return animationController_;
    }

    void SceneNode::OnScaleChange()
    {
        if (rigidBody_)
            rigidBody_->ReScale();
        else if (character_)
            character_->ReScale();
    }

    void SceneNode::OnHide(bool hide)
    {
        if (mesh_ && !hide)
        {
            auto scene = GetScene();
            if (scene)
                scene->UpdateOctree(this);
        }
        else if (hide)
        {
            auto scene = GetScene();
            if (scene)
                scene->RemoveFromOctree(this);
        }
    }

    void SceneNode::OnDirty() const
    {
        if (material_)
            material_->BachedNodeHasChanged();

        worldBBNeedsUpdate_ = true;
        auto scene = GetScene();
        if (scene)
            scene->NeedUpdate((SceneNode*)this);

        if(rigidBody_ && rigidBody_->IsKinematic())
            rigidBody_->SyncWithNode();
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

	bool SceneNode::IsBillboard() const
	{
		return material_ && material_->GetBillboardType() != BillboardType::NONE;
	}

    BoundingBox SceneNode::GetWorldBoundingBoxBut(const SceneNode* node) const
    {
        if (node == this)
            return BoundingBox();
        BoundingBox bb(GetWorldBoundingBox());
        for (auto& obj : children_)
        {
            auto child = dynamic_cast<SceneNode*>(obj.get());
            if (child)
                bb.Merge(child->GetWorldBoundingBoxBut(node));
        }
        return bb;
    }

    void SceneNode::GetMaterials(std::vector<PMaterial>& materials) const
    {
        materials.push_back(material_);
        for (auto& obj : children_)
        {
            auto child = dynamic_cast<SceneNode*>(obj.get());
            if (child)
                child->GetMaterials(materials);
        }
    }

    void SceneNode::Save(pugi::xml_node& node) const
    {
        if (!IsSerializable())
            return;
        Node::Save(node);
        if (skeleton_)
            node.append_attribute("skeleton").set_value(skeleton_->GetName().c_str());
        node.append_attribute("nodeType").set_value("SceneNode");
        if (material_)
            node.append_attribute("materialName").set_value(material_->GetName().c_str());
        if (mesh_)
            node.append_attribute("meshName").set_value(mesh_->GetName().c_str());
        if (rigidBody_)
            rigidBody_->Save(node);
        else if (character_)
            character_->Save(node);
        SaveChildren(node);
    }

    void SceneNode::SaveChildren(pugi::xml_node& node) const
    {
        for (auto& obj : children_)
        {
            auto sceneNode = std::dynamic_pointer_cast<SceneNode>(obj);
            if (sceneNode)
            {
                pugi::xml_node child = node.append_child("SceneNode");
                sceneNode->Save(child);
            }
        }
    }

    void SceneNode::Load(const pugi::xml_node& node)
    {
        Node::Load(node);
        pugi::xml_attribute attribute = node.attribute("materialName");
        if (attribute)
        {
            std::string name = attribute.as_string();
            SetMaterial(Material::Get(name));
        }

        attribute = node.attribute("hide");
        if (attribute)
            Hide(true);

        attribute = node.attribute("meshName");
        if (attribute)
        {
            std::string name = attribute.as_string();
            SetMesh(Mesh::Get(name));
        }

        pugi::xml_node childRigidBody = node.child("RigidBody");
        if (childRigidBody)
        {
            auto obj = GetOrCreateRigidBody();
            obj->Load(childRigidBody);
        }
        else
        {
            pugi::xml_node childCharacter = node.child("Character");
            if (childCharacter)
            {
                auto obj = GetOrCreateCharacter();
                obj->Load(childCharacter);
            }
        }

        LoadChildren(node);

        auto att = node.attribute("skeleton");
        if (att)
        {
            auto name = att.as_string();
            auto skeleton = Skeleton::Get(name);
            CHECK_ASSERT(skeleton);
            SetSkeleton(skeleton);
        }
    }

    void SceneNode::SetArmature(PSceneNode armature)
    {
        armature_ = armature;
        for (auto obj : children_)
        {
            auto sceneNode = dynamic_pointer_cast<SceneNode>(obj);
            if (sceneNode->mesh_ && sceneNode->mesh_->HasDeformBones())
                sceneNode->SetArmature(armature);
        }
    }

    PSceneNode SceneNode::GetArmature() const
    {
        return armature_.lock();
    }

    void SceneNode::LoadChildren(const pugi::xml_node& node)
    {
        pugi::xml_node child = node.child("SceneNode");
        while (child)
        {
            std::string childName = child.attribute("name").as_string();
            if (childName.empty())
                break;
            std::string nodeType = child.attribute("nodeType").as_string();
            //CHECK_ASSERT(!nodeType.empty());
            if (nodeType == "Light")
            {
                Node::CreateChild<Light>(childName, child);
            }
            else if (nodeType == "Camera")
            {
                PCamera childNode = Node::GetOrCreateChild<Camera>(childName);
                childNode->Load(child);
            }
            else
            {
                //CHECK_ASSERT(nodeType == "SceneNode");
                PSceneNode childNode = Node::GetOrCreateChild<SceneNode>(childName);
                childNode->Load(child);
            }

            child = child.next_sibling("SceneNode");
        }
    }

    void SceneNode::OnCollision(const ContactPoint& contactInfo)
    {
        signalCollision_->Run(contactInfo);
    }

    void SceneNode::SetFlags(const SceneNodeFlags& flags)
    {
        if (flags_ != flags)
        {
            flags_ = flags;
        }
    }

    void SceneNode::EnableFlags(const SceneNodeFlags& flags)
    {
        SetFlags(flags_ | (int)flags);
    }

    void SceneNode::DisableFlags(const SceneNodeFlags& flags)
    {
        SetFlags(flags_ & ~(int)flags);
    }

    void SceneNode::SetSkeleton(PSkeleton skeleton)
    {
        if (skeleton_ != skeleton)
        {
            skeleton_ = skeleton;
            if (skeleton_)
            {
				auto thisSceneNode = std::dynamic_pointer_cast<SceneNode>(SharedFromPointerNode(this));
                SetArmature(thisSceneNode);
                CHECK_CONDITION(skeleton_->IsReady());
                skeleton_->CreateBonesFor(thisSceneNode);
            }
        }
    }

    size_t SceneNode::GetMaxPlatformBones(size_t nBones) const
    {
        // set a maximum value per platform to avoid shader variations
        if (nBones <= MAX_BONES2)
            return MAX_BONES2;
        else if (nBones <= MAX_BONES1)
            return MAX_BONES1;
        else if (nBones <= MAX_BONES0)
            return MAX_BONES0;
        return nBones;
    }

    void SceneNode::FillShaderDefines(std::string& defines) const
    {
        auto armature = GetArmature();
        if (armature)
        {
            auto skeleton = armature->GetSkeleton();
            CHECK_CONDITION(skeleton->IsReady());
            defines += "SKELETON_" + skeleton->GetName() + "\n"; // just to have a shader variance per skeleton
            auto nBones = skeleton->GetNumberOfBones();
            if (nBones)
            {
                defines += "MAX_BONES " + ToString(GetMaxPlatformBones(nBones)) + "\n";
                defines += "SKINNED\n";
            }
        }
    }
}
