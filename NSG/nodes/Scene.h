/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "BoundingBox.h"
#include "Light.h"
#include "Overlay.h"
#include "SceneNode.h"
#include "SharedPointers.h"
#include "Types.h"
#include "Util.h"
#include <set>

namespace NSG {
class Scene : public SceneNode {
public:
    Scene(const std::string& name = GetUniqueName("scene"));
    ~Scene();
    void SetWindow(PWindow window);
    void SetAmbientColor(const Color& ambient);
    void SetHorizonColor(const Color& horizon);
    const Color& GetAmbientColor() const { return ambient_; }
    const Color& GetHorizonColor() const { return horizon_; }
    const std::vector<Light*>& GetLights() const;
    std::vector<Camera*> GetCameras() const;
    void UpdateAll(float deltaTime);
    void NeedUpdate(SceneNode* obj);
    void GetVisibleNodes(const Camera* camera,
                         std::vector<SceneNode*>& visibles) const;
    void GetVisibleNodes(const Frustum* frustum,
                         std::vector<SceneNode*>& visibles) const;
    void Save(pugi::xml_node& node) const override;
    void Load(const pugi::xml_node& node) override;
    bool GetFastRayNodesIntersection(const Ray& ray,
                                     std::vector<SceneNode*>& nodes) const;
    bool
    GetPreciseRayNodesIntersection(const Ray& ray,
                                   std::vector<RayNodeResult>& result) const;
    bool GetClosestRayNodeIntersection(const Ray& ray,
                                       RayNodeResult& closest) const;
    bool GetVisibleBoundingBox(const Camera* camera, BoundingBox& bb) const;
    PPhysicsWorld GetPhysicsWorld() const { return physicsWorld_; }
    SceneNode* GetClosestNode(const Camera* camera, float screenX,
                              float screenY) const;
    SignalNodeMouseMoved::PSignal SigNodeMouseMoved() {
        return signalNodeMouseMoved_;
    }
    SignalNodeMouseButton::PSignal SigNodeMouseDown() {
        return signalNodeMouseDown_;
    }
    SignalNodeMouseButton::PSignal SigNodeMouseUp() {
        return signalNodeMouseUp_;
    }
    SignalNodeMouseMoved::PSignal SigNodeMouseWheel() {
        return signalNodeMouseWheel_;
    }
    SignalUpdate::PSignal SigUpdate() { return signalUpdate_; }
    unsigned GetDrawablesNumber() const;
    PCamera GetMainCamera() const;
    void SetMainCamera(PCamera camera);
    const std::vector<SceneNode*>& GetDrawables() const;
    void EnableFog(bool enable);
    void SetFogMinIntensity(
        float intensity); // same as density (so far not used by the shader)
    void SetFogStart(float start);
    void SetFogDepth(float depth);
    void SetFogHeight(float height);
    float GetFogMinIntensity()
        const; // same as density (so far not used by the shader)
    float GetFogStart() const;
    float GetFogDepth() const;
    float GetFogHeight() const;
    void FillShaderDefines(std::string& defines, PassType passType) const;
    POverlay CreateOverlay(const std::string& name);
    POverlay GetOverlay(const std::string& name);
    POverlay GetOrCreateOverlay(const std::string& name);
    void RemoveOverlay(const std::string& name);
    PScene GetOverlays() const { return overlays_; }
    void RemoveLight(Light* light);
    void RemoveCamera(Camera* camera);
    void RemoveParticleSystem(ParticleSystem* ps);
    static constexpr float MAX_WORLD_SIZE = 5000.f;

protected:
    void LoadPhysics(const pugi::xml_node& node);
    void SavePhysics(pugi::xml_node& node) const;
    void AddLight(Light* light);
    void AddCamera(Camera* camera);
    void AddParticleSystem(ParticleSystem* ps);
    void UpdateOctree(SceneNode* node);
    void RemoveFromOctree(SceneNode* node);

private:
    void UpdateParticleSystems(float deltaTime);

private:
    Camera* mainCamera_;
    std::vector<Camera*> cameras_;
    std::vector<Light*> lights_;
    std::vector<ParticleSystem*> particleSystems_;
    Color ambient_;
    Color horizon_;
    POctree octree_;
    mutable std::set<SceneNode*> octreeNeedsUpdate_;
    PPhysicsWorld physicsWorld_;
    PWeakWindow window_;
    SignalNodeMouseMoved::PSignal signalNodeMouseMoved_;
    SignalNodeMouseButton::PSignal signalNodeMouseDown_;
    SignalNodeMouseButton::PSignal signalNodeMouseUp_;
    SignalNodeMouseMoved::PSignal signalNodeMouseWheel_;
    SignalUpdate::PSignal signalUpdate_;
    SignalFloatFloat::PSlot slotMouseMoved_;
    SignalMouseButton::PSlot slotMouseDown_;
    SignalMouseButton::PSlot slotMouseUp_;
    SignalFloatFloat::PSlot slotMouseWheel_;
    bool enableFog_;
    float fogMinIntensity_;
    float fogStart_;
    float fogDepth_;
    float fogHeight_;
    PScene overlays_;
    friend class Node;
    friend class SceneNode;
};
}
