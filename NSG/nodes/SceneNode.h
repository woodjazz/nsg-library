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
#pragma once
#include "Types.h"
#include "Node.h"
#include "BoundingBox.h"
#include "SignalSlots.h"
#include <string>
#include <vector>

namespace NSG
{
	class Octant;
	class SceneNode : public Node
	{
	public:
		SceneNode(const std::string& name);
		~SceneNode();
		virtual bool CanBeVisible() const;
		PMaterial GetMaterial() const { return material_; }
		void SetMaterial(PMaterial material);
		void SetMesh(PMesh mesh);
		PRigidBody GetOrCreateRigidBody();
		PCharacter GetOrCreateCharacter();
		PRigidBody GetRigidBody() const { return rigidBody_; }
		PCharacter GetCharacter() const { return character_; }
		PAnimationController GetOrCreateAnimationController();
		PMesh GetMesh() const { return mesh_; }
		void SetOctant(Octant* octant) const { octant_ = octant; }
		const BoundingBox& GetWorldBoundingBox() const;
		BoundingBox GetWorldBoundingBoxBut(const SceneNode* node) const;
		Octant* GetOctant() const { return octant_; }
		virtual void OnDirty() const override;
		void OnScaleChange() override;
        virtual void OnCollision(const ContactPoint& contactInfo);
		void OnHide(bool hide) override;
		void SetSerializable(bool serializable) { serializable_ = serializable; }
		bool IsSerializable() const { return serializable_;  }
		void GetMaterials(std::vector<PMaterial>& materials) const;
		void Load(const pugi::xml_node& node) override;
		void Save(pugi::xml_node& node) const override;
		void SaveChildren(pugi::xml_node& node) const;
		void LoadChildren(const pugi::xml_node& node);
		SignalCollision::PSignal SigCollision() { return signalCollision_; }
		SignalEmpty::PSignal SigMeshSet() { return signalMeshSet_; }
		SignalEmpty::PSignal SigMaterialSet() { return signalMaterialSet_; }
		const SceneNodeFlags& GetFlags() const { return flags_; }
        void SetFlags(const SceneNodeFlags& flags);
        void EnableFlags(const SceneNodeFlags& flags);
        void DisableFlags(const SceneNodeFlags& flags);
		bool AllowRayQuery() const { return flags_ & (int)SceneNodeFlag::ALLOW_RAY_QUERY; }
		void SetSkeleton(PSkeleton skeleton);
		PSkeleton GetSkeleton() const { return skeleton_; }
		void FillShaderDefines(std::string& defines) const;
		size_t GetMaxPlatformBones(size_t nBones) const;
		PSceneNode GetArmature() const;
		void SetArmature(PSceneNode armature);
		bool IsBillboard() const;
	protected:
		PMaterial material_;
		PMesh mesh_;
		PSkeleton skeleton_;
	private:
		PWeakSceneNode armature_;
		PRigidBody rigidBody_;
		PCharacter character_;
		PAnimationController animationController_;
		mutable Octant* octant_;
		mutable BoundingBox worldBB_;
		mutable bool worldBBNeedsUpdate_;
		bool serializable_;
		SceneNodeFlags flags_;
		SignalEmpty::PSignal signalMeshSet_;
		SignalEmpty::PSignal signalMaterialSet_;
		SignalCollision::PSignal signalCollision_;
	};
}
