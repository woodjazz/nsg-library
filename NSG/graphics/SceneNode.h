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
#include "GPUObject.h"
#include <string>
#include <vector>

namespace NSG
{
	class Octant;
	class SceneNode : public Node, public GPUObject
	{
	public:
		~SceneNode();
		void Set(PMaterial material);
		void Set(PMesh mesh);
		PMesh GetMesh() const { return mesh_; }
		void SetBehavior(PBehavior behavior);
		void SetOctant(Octant* octant) const { octant_ = octant; }
		const BoundingBox& GetWorldBoundingBox() const;
		bool IsOccludee() const { return occludee_; }
		Octant* GetOctant() const { return octant_; }
		virtual void OnDirty() const override;
		virtual void Save(pugi::xml_node& node) override;
		void Load(const pugi::xml_document& doc, const std::string& name);
		void SetMeshIndex(int index) {meshIndex_ = index;}
		void SetMaterialIndex(int index) {materialIndex_ = index;}
		virtual bool IsValid() override;
		virtual void AllocateResources() override;
		void Start();
		void Update();
		void Render();
	protected:
		SceneNode(const std::string& name, Scene* scene);
		SceneNode(PResource resource, const std::string& name, Scene* scene);
	private:
		PSceneNode CreateChild();
		struct CachedData
		{
			std::vector<PMesh> meshes_;
			std::vector<PMaterial> materials_;
		};
		void LoadMeshesAndMaterials(const pugi::xml_document& doc, CachedData& data);
		void Load(const pugi::xml_node& child, const CachedData& data);
		PMaterial material_;
		PMesh mesh_;
		PBehavior behavior_;
		mutable Octant* octant_;
		mutable BoundingBox worldBB_;
		bool occludee_;
		mutable bool worldBBNeedsUpdate_;
		PResource resource_;
		int meshIndex_;
		int materialIndex_;
		Scene* scene_;
		friend class Scene;
	};
}
