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
#include "SceneNode.h"
#include "Light.h"
#include "BoundingBox.h"
#include "MapAndVector.h"
#include "Util.h"
#include <set>
#include <map>

namespace NSG
{
    class Scene : public SceneNode
    {
    public:
		Scene(const std::string& name = GetUniqueName("scene"));
        ~Scene();
        void SetWindow(Window* window);
        void SetAmbientColor(Color ambient);
        void SetHorizonColor(Color horizon);
        const Color& GetAmbientColor() const { return ambient_; }
        const Color& GetHorizonColor() const { return horizon_; }
		void AddLight(Light* light);
		const std::vector<Light*>& GetLights() const;
        void AddCamera(Camera* camera);
		void AddParticleSystem(ParticleSystem* ps);
        void UpdateAll(float deltaTime);
		void Render();
        void NeedUpdate(SceneNode* obj);
		void GetVisibleNodes(const Camera* camera, std::vector<SceneNode*>& visibles) const;
		void GetVisibleNodes(const Frustum* frustum, std::vector<SceneNode*>& visibles) const;
		void Save(pugi::xml_node& node) const override;
        void Load(const pugi::xml_node& node) override;
        bool GetFastRayNodesIntersection(const Ray& ray, std::vector<SceneNode*>& nodes) const;
        bool GetPreciseRayNodesIntersection(const Ray& ray, std::vector<RayNodeResult>& result) const;
		bool GetClosestRayNodeIntersection(const Ray& ray, RayNodeResult& closest) const;
		bool GetVisibleBoundingBox(const Camera* camera, BoundingBox& bb) const;
		const std::map<std::string, PAnimation>& GetAnimations() const { return animations_; }
        PAnimation GetOrCreateAnimation(const std::string& name);
        std::vector<PAnimation> GetAnimationsFor(PNode node) const;
        bool HasAnimation(const std::string& name) const;
        bool PlayAnimation(const std::string& name, bool looped);
        void PlayAnimation(const PAnimation& animation, bool looped);
        bool SetAnimationSpeed(const std::string& name, float speed);
        PPhysicsWorld GetPhysicsWorld() const { return physicsWorld_; }
        void UpdateOctree(SceneNode* node);
        void RemoveFromOctree(SceneNode* node);
        SceneNode* GetClosestNode(float screenX, float screenY) const;
        SignalNodeMouseMoved::PSignal SigNodeMouseMoved() { return signalNodeMouseMoved_; }
        SignalNodeMouseButton::PSignal SigNodeMouseDown() { return signalNodeMouseDown_; }
        SignalNodeMouseButton::PSignal SigNodeMouseUp() { return signalNodeMouseUp_; }
        SignalNodeMouseMoved::PSignal SigNodeMouseWheel() { return signalNodeMouseWheel_; }
        unsigned GetDrawablesNumber() const;
        std::vector<Camera*> GetCameras() const;
		PCamera GetMainCamera() const;
        void SetMainCamera(PCamera camera);
        const std::vector<SceneNode*>& GetDrawables() const;
	protected:
		void LoadPhysics(const pugi::xml_node& node);
		void LoadAnimations(const pugi::xml_node& node);
		void LoadSkeletons(const pugi::xml_node& node);
		void SaveAnimations(pugi::xml_node& node) const;
		void SavePhysics(pugi::xml_node& node) const;
		void SaveSkeletons(pugi::xml_node& node) const;
    private:
        void UpdateAnimations(float deltaTime);
        void UpdateParticleSystems(float deltaTime);
    private:
        Camera* mainCamera_;
        std::vector<Camera*> cameras_;
		std::vector<Light*> lights_;
		std::vector<ParticleSystem*> particleSystems_;
        Color ambient_;
        Color horizon_;
		POctree octree_;
        mutable std::set<SceneNode*> needUpdate_;
		std::map<std::string, PAnimation> animations_;
        typedef std::map<std::string, PAnimationState> AnimationStateMap;
        AnimationStateMap animationStateMap_;
        PPhysicsWorld physicsWorld_;
        Window* window_;
        SignalNodeMouseMoved::PSignal signalNodeMouseMoved_;
        SignalNodeMouseButton::PSignal signalNodeMouseDown_;
        SignalNodeMouseButton::PSignal signalNodeMouseUp_;
        SignalNodeMouseMoved::PSignal signalNodeMouseWheel_;
		SignalFloatFloat::PSlot slotMouseMoved_;
		SignalMouseButton::PSlot slotMouseDown_;
		SignalMouseButton::PSlot slotMouseUp_;
		SignalFloatFloat::PSlot slotMouseWheel_;
		SignalWindow::PSlot slotWindowCreated_;
        SignalLight::PSlot slotLightBeingDestroy_;
		SignalCamera::PSlot slotCameraBeingDestroy_;
		SignalParticleSystem::PSlot slotPSBeingDestroy_;
    };
}