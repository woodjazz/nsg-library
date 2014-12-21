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
#include "Types.h"
#include "Node.h"
#include "BoundingBox.h"
#include "Object.h"
#include "SignalSlots.h"
#include <string>
#include <vector>

namespace NSG
{
	class Octant;
	class SceneNode : public Node, public Object
	{
	public:
		SceneNode(const std::string& name);
		virtual ~SceneNode();
		PMaterial GetMaterial() const { return material_; }
		void SetMaterial(PMaterial material);
		void SetMesh(PMesh mesh);
		PRigidBody GetOrCreateRigidBody();
		PMesh GetMesh() const { return mesh_; }
		void SetOctant(Octant* octant) const { octant_ = octant; }
		const BoundingBox& GetWorldBoundingBox() const;
		BoundingBox GetWorldBoundingBoxBut(const SceneNode* node) const;
		bool IsOccludee() const { return occludee_; }
		Octant* GetOctant() const { return octant_; }
		virtual void OnDirty() const override;
		void OnScaleChange() override;
        virtual void OnCollision(const ContactPoint& contactInfo);
		void OnEnable() override;
		void OnDisable() override;
		void SetSerializable(bool serializable) { serializable_ = serializable; }
		bool IsSerializable() const { return serializable_;  }
		void Load(PResource resource);
		void GetMaterials(std::vector<PMaterial>& materials) const;
		void Save(pugi::xml_node& node) override;
		void SaveChildren(pugi::xml_node& node);
		virtual void Load(const pugi::xml_node& child, const CachedData& data);
		void LoadChildren(const pugi::xml_node& node, const CachedData& data);
		void Render();
	public:
		SignalCollision::PSignal signalCollision_;
	protected:
		virtual bool IsValid() override;
		virtual void Load(const pugi::xml_document& doc, const CachedData& data);
		void LoadMeshesAndMaterials(const pugi::xml_document& doc, CachedData& data);
		App& app_;
	private:
		PMaterial material_;
		PMesh mesh_;
		PRigidBody rigidBody_;
		mutable Octant* octant_;
		mutable BoundingBox worldBB_;
		bool occludee_;
		mutable bool worldBBNeedsUpdate_;
		bool serializable_;
	};
}