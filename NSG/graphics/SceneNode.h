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
		PMaterial GetMaterial() const { return material_; }
		void Set(PMaterial material);
		void Set(PMesh mesh);
		void Set(PRigidBody rigidBody);
		PMesh GetMesh() const { return mesh_; }
		void AddBehavior(PBehavior behavior);
		void SetOctant(Octant* octant) const { octant_ = octant; }
		const BoundingBox& GetWorldBoundingBox() const;
		BoundingBox GetWorldBoundingBoxBut(const SceneNode* node) const;
		bool IsOccludee() const { return occludee_; }
		Octant* GetOctant() const { return octant_; }
		virtual void OnDirty() const override;
		void OnScaleChange() override;
		virtual void Save(pugi::xml_node& node) override;
		virtual bool IsValid() override;
		virtual void AllocateResources() override;
		virtual void Start() override;
		virtual void Update() override;
        virtual void ViewChanged(int width, int height) override;
        virtual void OnMouseMove(float x, float y) override;
		virtual void OnMouseDown(int button, float x, float y) override;
        virtual void OnMouseWheel(float x, float y) override;
		virtual void OnMouseUp(int button, float x, float y) override;
		virtual void OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers) override;
        virtual void OnKey(int key, int action, int modifier) override;
        virtual void OnChar(unsigned int character) override;
        virtual void OnCollision(const ContactPoint& contactInfo) override;
		PSceneNode CreateChild(const std::string& name);
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		void SetSerializable(bool serializable) { serializable_ = serializable; }
		bool IsSerializable() const { return serializable_;  }
		void SetResource(PResource resource);
		void SetDiffuseMap(PTexture texture, bool recursive);
		void SetNormalMap(PTexture texture, bool recursive);
		void SetLightMap(PTexture texture, bool recursive);
		void GetMaterials(std::vector<PMaterial>& materials) const;
	protected:
		virtual void Load(const pugi::xml_document& doc, const CachedData& data);
		void LoadMeshesAndMaterials(const pugi::xml_document& doc, CachedData& data);
		SceneNode(const std::string& name);
		App& app_;
	private:
		void LoadNode(const pugi::xml_node& child, const CachedData& data);
		PScene GetScene() const;
		PMaterial material_;
		PMesh mesh_;
		PRigidBody rigidBody_;
		std::vector<PBehavior> behaviors_;
		mutable Octant* octant_;
		mutable BoundingBox worldBB_;
		bool occludee_;
		mutable bool worldBBNeedsUpdate_;
		PResource resource_;
		PScene scene_;
		bool serializable_;
		friend class Scene;
	};
}
