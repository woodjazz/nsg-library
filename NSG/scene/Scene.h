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
#include "SceneNode.h"
#include "Light.h"
#include "BoundingBox.h"
#include "MapAndVector.h"
#include <set>
#include <map>

namespace NSG
{
    class Scene : public SceneNode
    {
    public:
        Scene(const std::string& name);
        ~Scene();
        void SetAmbientColor(Color ambient);
        const Color& GetAmbientColor() const { return ambient_; }
		void AddLight(PLight light);
        void ChangeLightType(PLight light, LightType fromType);
		const std::vector<PWeakLight>& GetLights(LightType type) const;
        void AddCamera(PCamera camera);
        const std::vector<PWeakCamera>& GetCameras() const;
        void Update(float deltaTime);
		void Render(Camera* camera);
        void NeedUpdate(SceneNode* obj);
        void GetVisibleNodes(const Camera* camera, std::vector<SceneNode*>& visibles) const;
        POctree GetOctree() const { return octree_; }
		void Save(pugi::xml_node& node) const override;
        void Load(const pugi::xml_node& node) override;
        bool GetFastRayNodesIntersection(const Ray& ray, std::vector<SceneNode*>& nodes) const;
        bool GetPreciseRayNodesIntersection(const Ray& ray, std::vector<RayNodeResult>& result) const;
        bool GetClosestRayNodeIntersection(const Ray& ray, RayNodeResult& closest);
        bool GetVisibleBoundingBox(const Camera* camera, BoundingBox& bb) const;
        PAnimation GetOrCreateAnimation(const std::string& name);
        std::vector<PAnimation> GetAnimationsFor(PNode node) const;
        bool HasAnimation(const std::string& name) const;
        bool PlayAnimation(const std::string& name, bool looped);
        void PlayAnimation(const PAnimation& animation, bool looped);
        bool SetAnimationSpeed(const std::string& name, float speed);
        PPhysicsWorld GetPhysicsWorld() const { return physicsWorld_; }
    protected:
		void LoadPhysics(const pugi::xml_node& node);
		void LoadAnimations(const pugi::xml_node& node);
		void LoadSkeletons(const pugi::xml_node& node);
		void SaveAnimations(pugi::xml_node& node) const;
		void SavePhysics(pugi::xml_node& node) const;
		void SaveSkeletons(pugi::xml_node& node) const;
    private:
        void UpdateAnimations(float deltaTime);
		bool HasLight(PLight light) const;
    private:
		mutable std::map<LightType, std::vector<PWeakLight>> lights_;
        std::vector<PWeakCamera> cameras_;
        Color ambient_;
        POctree octree_;
        mutable std::set<SceneNode*> needUpdate_;
        App& app_;
		MapAndVector<std::string, Animation> animations_;
        typedef std::map<std::string, PAnimationState> AnimationStateMap;
        AnimationStateMap animationStateMap_;
        PPhysicsWorld physicsWorld_;
    };
}