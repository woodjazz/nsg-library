#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "Light.h"
#include "Octree.h"
#include "OctreeQuery.h"
#include "RenderingContext.h"
#include "Material.h"
#include "Mesh.h"
#include "Util.h"
#include "Skeleton.h"
#include "ModelMesh.h"
#include "PhysicsWorld.h"
#include "ParticleSystem.h"
#include "Window.h"
#include "StringConverter.h"
#include "Check.h"
#include "SharedFromPointer.h"
#include "pugixml.hpp"
#include <algorithm>
#include <functional>
#include <thread>

namespace NSG
{
    Scene::Scene(const std::string& name)
        : SceneNode(name),
          mainCamera_(nullptr),
          ambient_(0.3f, 0.3f, 0.3f, 1.f),
          horizon_(0.f, 0.f, 0.f, 1.f),
          octree_(std::make_shared<Octree>()),
          signalNodeMouseMoved_(new Signal<SceneNode *, float, float>()),
          signalNodeMouseDown_(new Signal<SceneNode *, int, float, float>()),
          signalNodeMouseUp_(new Signal<SceneNode *, int, float, float>()),
          signalNodeMouseWheel_(new Signal<SceneNode *, float, float>()),
          signalUpdate_(new Signal<float>()),
          enableFog_(false),
          fogMinIntensity_(0),
          fogStart_(5),
          fogDepth_(25),
          fogHeight_(0)
    {
        slotLightBeingDestroy_ = Light::SignalBeingDestroy()->Connect([this](Light * light)
        {
            auto it = std::find(lights_.begin(), lights_.end(), light);
            if (it != lights_.end())
                lights_.erase(it);
        });

        slotCameraBeingDestroy_ = Camera::SignalBeingDestroy()->Connect([this](Camera * camera)
        {
            if (mainCamera_ == camera)
                mainCamera_ = nullptr;
            auto it = std::find(cameras_.begin(), cameras_.end(), camera);
            if (it != cameras_.end())
                cameras_.erase(it);
        });

        slotPSBeingDestroy_ = ParticleSystem::SignalBeingDestroy()->Connect([this](ParticleSystem * ps)
        {
            auto it = std::find(particleSystems_.begin(), particleSystems_.end(), ps);
            if (it != particleSystems_.end())
                particleSystems_.erase(it);
        });

        physicsWorld_ = std::make_shared<PhysicsWorld>(this);
    }

    Scene::~Scene()
    {
        physicsWorld_ = nullptr;
        slotLightBeingDestroy_ = nullptr;
        slotCameraBeingDestroy_ = nullptr;
        slotPSBeingDestroy_ = nullptr;
    }

    void Scene::SetWindow(PWindow window)
    {
        if (window_.lock() != window)
        {
            if (window)
            {
                slotMouseMoved_ = window->SigFloatFloat()->Connect([this](float x, float y)
                {
                    if (signalNodeMouseMoved_->HasSlots())
                    {
                        SceneNode* node = GetClosestNode(mainCamera_, x, y);
                        if (node)
                            signalNodeMouseMoved_->Run(node, x, y);
                    }
                });

                slotMouseDown_ = window->SigMouseDown()->Connect([this](int button, float x, float y)
                {
                    if (signalNodeMouseDown_->HasSlots())
                    {
                        SceneNode* node = GetClosestNode(mainCamera_, x, y);
                        if (node)
                            signalNodeMouseDown_->Run(node, button, x, y);
                    }
                });

                slotMouseUp_ = window->SigMouseUp()->Connect([this](int button, float x, float y)
                {
                    if (signalNodeMouseUp_->HasSlots())
                    {
                        SceneNode* node = GetClosestNode(mainCamera_, x, y);
                        if (node)
                            signalNodeMouseUp_->Run(node, button, x, y);
                    }
                });

                slotMouseWheel_ = window->SigMouseWheel()->Connect([this](float x, float y)
                {
                    if (signalNodeMouseWheel_->HasSlots())
                    {
                        SceneNode* node = GetClosestNode(mainCamera_, x, y);
                        if (node)
                            signalNodeMouseWheel_->Run(node, x, y);
                    }
                });

            }
            else
            {
                slotMouseMoved_ = nullptr;
                slotMouseDown_ = nullptr;
                slotMouseUp_ = nullptr;
                slotMouseWheel_ = nullptr;
            }

            window_ = window;
        }
    }

    void Scene::SetAmbientColor(const Color& ambient)
    {
        if (ambient_ != ambient)
        {
            ambient_ = ambient;
            SetUniformsNeedUpdate();
        }
    }

    void Scene::SetHorizonColor(const Color& horizon)
    {
        if (horizon_ != horizon)
        {
            horizon_ = horizon;
            SetUniformsNeedUpdate();
        }
    }

    void Scene::UpdateAll(float deltaTime)
    {
        physicsWorld_->StepSimulation(deltaTime);
        UpdateParticleSystems(deltaTime);
        signalUpdate_->Run(deltaTime);
    }

    bool Scene::GetFastRayNodesIntersection(const Ray& ray, std::vector<SceneNode*>& nodes) const
    {
        RayOctreeQuery query(nodes, ray);
        octree_->Execute(query);
        return !nodes.empty();
    }

    bool Scene::GetPreciseRayNodesIntersection(const Ray& ray, std::vector<RayNodeResult>& result) const
    {
        std::vector<SceneNode*> tmpNodes;
        RayOctreeQuery query(tmpNodes, ray);
        octree_->Execute(query);
        result.clear();
        float maxDistance = ray.GetMaxDistance();
        for (auto& obj : tmpNodes)
        {
            if (obj->IsBillboard())
            {
                auto distance = obj->GetGlobalPosition().Distance(ray.GetOrigin());
                RayNodeResult r {distance, obj};
                result.push_back(r);
            }
            else
            {
                float distance = ray.HitDistance(obj);
                if (distance < maxDistance)
                {
                    RayNodeResult r {distance, obj};
                    result.push_back(r);
                }
            }
        }
        return !result.empty();
    }

    bool Scene::GetClosestRayNodeIntersection(const Ray& ray, RayNodeResult& closest) const
    {
        std::vector<RayNodeResult> results;
        if (GetPreciseRayNodesIntersection(ray, results))
        {
            int closestIdx = -1;
            int idx = 0;
            float distance = std::numeric_limits<float>::max();
            for (auto& result : results)
            {
                if (result.distance_ < distance)
                {
                    closestIdx = idx;
                    distance = result.distance_;
                }
                ++idx;
            }

            if (closestIdx != -1)
            {
                closest = results[closestIdx];
                return true;
            }
        }
        return false;
    }

    void Scene::GetVisibleNodes(const Camera* camera, std::vector<SceneNode*>& visibles) const
    {
        for (auto& obj : octreeNeedsUpdate_)
            octree_->InsertUpdate(obj);
        octreeNeedsUpdate_.clear();
        FrustumOctreeQuery query(visibles, camera->GetFrustum().get());
        octree_->Execute(query);
    }

    void Scene::GetVisibleNodes(const Frustum* frustum, std::vector<SceneNode*>& visibles) const
    {
        for (auto& obj : octreeNeedsUpdate_)
            octree_->InsertUpdate(obj);
        octreeNeedsUpdate_.clear();
        FrustumOctreeQuery query(visibles, frustum);
        octree_->Execute(query);
    }

    void Scene::NeedUpdate(SceneNode* obj)
    {
        if (obj->GetMesh() != nullptr && !obj->IsHidden())
            octreeNeedsUpdate_.insert(obj);
    }

    void Scene::SavePhysics(pugi::xml_node& node) const
    {
        pugi::xml_node child = node.append_child("Physics");
        child.append_attribute("gravity").set_value(ToString(physicsWorld_->GetGravity()).c_str());
        child.append_attribute("fps").set_value(physicsWorld_->GetFps());
        child.append_attribute("maxSubSteps").set_value(physicsWorld_->GetMaxSubSteps());
    }

    void Scene::LoadPhysics(const pugi::xml_node& node)
    {
        pugi::xml_node child = node.child("Physics");
        Vertex3 gravity = ToVertex3(child.attribute("gravity").as_string());
        physicsWorld_->SetGravity(gravity);
        physicsWorld_->SetFps(child.attribute("fps").as_int());
        physicsWorld_->SetMaxSubSteps(child.attribute("maxSubSteps").as_int());
    }

    void Scene::Load(const pugi::xml_node& node)
    {
        SetAmbientColor(ToVertex3(node.attribute("ambient").as_string()));
        SetHorizonColor(ToVertex3(node.attribute("horizon").as_string()));
        EnableFog(node.attribute("enableFog").as_bool());
        SetFogMinIntensity(node.attribute("fogMinIntensity").as_float());
        SetFogStart(node.attribute("fogStart").as_float());
        SetFogDepth(node.attribute("fogDepth").as_float());
        SetFogHeight(node.attribute("fogHeight").as_float());
        std::string mainCameraName = node.attribute("mainCamera").as_string();
        pugi::xml_node sceneNode = node.child("SceneNode");
        CHECK_ASSERT(sceneNode);
        SceneNode::Load(sceneNode);
        if (!mainCameraName.empty())
            SetMainCamera(GetChild<Camera>(mainCameraName, true));
        LoadPhysics(node);
    }

    void Scene::Save(pugi::xml_node& node) const
    {
        pugi::xml_node scene = node.append_child("Scene");
        scene.append_attribute("ambient").set_value(ToString(ambient_).c_str());
        scene.append_attribute("horizon").set_value(ToString(horizon_).c_str());
        scene.append_attribute("enableFog").set_value(enableFog_);
        scene.append_attribute("fogMinIntensity").set_value(fogMinIntensity_);
        scene.append_attribute("fogStart").set_value(fogStart_);
        scene.append_attribute("fogDepth").set_value(fogDepth_);
        scene.append_attribute("fogHeight").set_value(fogHeight_);
        std::string mainCameraName;
        if (mainCamera_)
            mainCameraName = mainCamera_->GetName();
        scene.append_attribute("mainCamera").set_value(mainCameraName.c_str());
        pugi::xml_node sceneNode = scene.append_child("SceneNode");
        SceneNode::Save(sceneNode);
        SavePhysics(scene);
    }

    bool Scene::GetVisibleBoundingBox(const Camera* camera, BoundingBox& bb) const
    {
        std::vector<SceneNode*> visibles;
        GetVisibleNodes(camera, visibles);
        if (!visibles.empty())
        {
            bb = BoundingBox();
            for (auto& obj : visibles)
                bb.Merge(obj->GetWorldBoundingBox());
            return true;
        }
        return false;
    }

    void Scene::UpdateParticleSystems(float deltaTime)
    {
        for (auto ps : particleSystems_)
            ps->Update(deltaTime);
    }

    void Scene::AddLight(Light* light)
    {
        lights_.push_back(light);

        // we want the lights sorted by type for rendering/batching purposes
        std::sort(lights_.begin(), lights_.end(), [&](const Light * a, const Light * b) -> bool
        {
            return (int)a->GetType() < (int)b->GetType();
        });

        UpdateOctree(light);
    }

    const std::vector<Light*>& Scene::GetLights() const
    {
        return lights_;
    }

    void Scene::AddCamera(Camera* camera)
    {
        cameras_.push_back(camera);
        if (!mainCamera_)
            mainCamera_ = camera;
        UpdateOctree(camera);
    }

    void Scene::AddParticleSystem(ParticleSystem* ps)
    {
        particleSystems_.push_back(ps);
    }

    void Scene::UpdateOctree(SceneNode* node)
    {
        if (node->GetMesh())
            octree_->InsertUpdate(node);
    }

    void Scene::RemoveFromOctree(SceneNode* node)
    {
        octreeNeedsUpdate_.erase(node);
        octree_->Remove(node);
    }

    SceneNode* Scene::GetClosestNode(const Camera* camera, float screenX, float screenY) const
    {
        Ray ray = Camera::GetRay(camera, screenX, screenY);
        RayNodeResult closest{ 0, nullptr };
        if (GetClosestRayNodeIntersection(ray, closest))
            return closest.node_;
        return nullptr;
    }

    unsigned Scene::GetDrawablesNumber() const
    {
        return octree_->GetNumDrawables();
    }

    PCamera Scene::GetMainCamera() const
    {
        if (mainCamera_)
        {
            auto p = SharedFromPointerNode(mainCamera_);
            return std::dynamic_pointer_cast<Camera>(p);
        }
        return nullptr;
    }

    void Scene::SetMainCamera(PCamera camera)
    {
        mainCamera_ = camera.get();
    }

    const std::vector<SceneNode*>& Scene::GetDrawables() const
    {
        return octree_->GetDrawables();
    }

    void Scene::EnableFog(bool enable)
    {
        if (enableFog_ != enable)
        {
            enableFog_ = enable;
            SetUniformsNeedUpdate();
        }
    }

    void Scene::SetFogMinIntensity(float intensity)
    {
        if (fogMinIntensity_ != intensity)
        {
            fogMinIntensity_ = intensity;
            if (enableFog_)
                SetUniformsNeedUpdate();
        }
    }

    void Scene::SetFogStart(float start)
    {
        if (fogStart_ != start)
        {
            fogStart_ = start;
            if (enableFog_)
                SetUniformsNeedUpdate();
        }
    }

    void Scene::SetFogDepth(float depth)
    {
        if (fogDepth_ != depth)
        {
            fogDepth_ = depth;
            if (enableFog_)
                SetUniformsNeedUpdate();
        }
    }

    void Scene::SetFogHeight(float height)
    {
        if (fogHeight_ != height)
        {
            fogHeight_ = height;
            if (enableFog_)
                SetUniformsNeedUpdate();
        }
    }

    void Scene::FillShaderDefines(std::string& defines, PassType passType) const
    {
        if (enableFog_ && PassType::SHADOW != passType)
        {
            defines += "FOG\n";
            if (fogHeight_ != 0)
                defines += "FOGHEIGHT\n";
        }
    }

    float Scene::GetFogMinIntensity() const
    {
        return fogMinIntensity_;
    }

    float Scene::GetFogStart() const
    {
        return fogStart_;
    }

    float Scene::GetFogDepth() const
    {
        return fogDepth_;
    }

    float Scene::GetFogHeight() const
    {
        return fogHeight_;
    }

    POverlay Scene::CreateOverlay(const std::string& name)
    {
        if(!overlays_)
            overlays_ = std::make_shared<Scene>(GetUniqueName("NSGOverlays"));
        return overlays_->CreateChild<Overlay>(name);
    }

	POverlay Scene::GetOrCreateOverlay(const std::string& name)
    {
        if(!overlays_)
            overlays_ = std::make_shared<Scene>(GetUniqueName("NSGOverlays"));
        return overlays_->GetOrCreateChild<Overlay>(name);   
    }

    void Scene::RemoveOverlay(const std::string& name)
    {
        if(overlays_)
        {
            auto overlay = overlays_->GetChild<Overlay>(name, false);
            if(overlay)
                overlay->SetParent(nullptr);
        }            
    }    
}

