#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "Light.h"
#include "Octree.h"
#include "OctreeQuery.h"
#include "Graphics.h"
#include "Constants.h"
#include "Material.h"
#include "Mesh.h"
#include "App.h"
#include "Util.h"
#include "Skeleton.h"
#include "ModelMesh.h"
#include "Animation.h"
#include "AnimationState.h"
#include "PhysicsWorld.h"
#include "ParticleSystem.h"
#include "Window.h"
#include "pugixml.hpp"
#include <algorithm>
#include <functional>
#include <thread>

namespace NSG
{
    Scene::Scene(const std::string& name)
        : SceneNode(name),
          signalNodeMouseMoved_(new Signal<SceneNode *, float, float>()),
          signalNodeMouseDown_(new Signal<SceneNode *, int, float, float>()),
          signalNodeMouseUp_(new Signal<SceneNode *, int, float, float>()),
          signalNodeMouseWheel_(new Signal<SceneNode *, float, float>()),
          ambient_(0.3f, 0.3f, 0.3f, 1),
          octree_(new Octree),
          app_(*App::this_),
          physicsWorld_(new PhysicsWorld)
    {
        //octree_->InsertUpdate(this);
        Graphics::this_->SetScene(this);
        auto window = Graphics::this_->GetWindow();
        if(window)
            SetWindow(window);
    }

    Scene::~Scene()
    {
        octree_ = nullptr;
    }

    void Scene::SetWindow(Window* window)
    {
        if (window)
        {
			slotMouseMoved_ = window->signalMouseMoved_->Connect([&](float x, float y)
            {
                if (signalNodeMouseMoved_->HasSlots())
                {
                    SceneNode* node = GetClosestNode(x, y);
                    if (node)
                        signalNodeMouseMoved_->Run(node, x, y);
                }
            });

			slotMouseDown_ = window->signalMouseDown_->Connect([&](int button, float x, float y)
            {
                if (signalNodeMouseDown_->HasSlots())
                {
                    SceneNode* node = GetClosestNode(x, y);
                    if (node)
                        signalNodeMouseDown_->Run(node, button, x, y);
                }
            });

			slotMouseUp_ = window->signalMouseUp_->Connect([&](int button, float x, float y)
            {
                if (signalNodeMouseUp_->HasSlots())
                {
                    SceneNode* node = GetClosestNode(x, y);
                    if (node)
                        signalNodeMouseUp_->Run(node, button, x, y);
                }
            });

			slotMouseWheel_ = window->signalMouseWheel_->Connect([&](float x, float y)
            {
                if (signalNodeMouseWheel_->HasSlots())
                {
                    SceneNode* node = GetClosestNode(x, y);
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
    }


    void Scene::SetAmbientColor(Color ambient)
    {
        if (ambient_ != ambient)
        {
            ambient_ = ambient;
            SetUniformsNeedUpdate();
        }
    }

    void Scene::Update(float deltaTime)
    {
        physicsWorld_->StepSimulation(deltaTime);
        UpdateAnimations(deltaTime);
        UpdateParticleSystems(deltaTime);
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
            float distance = ray.HitDistance(obj);
            if (distance < maxDistance)
            {
                RayNodeResult r {distance, obj};
                result.push_back(r);
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

            closest = results[closestIdx];

            return true;

        }
        return false;
    }

    void Scene::GetVisibleNodes(const Camera* camera, std::vector<SceneNode*>& visibles) const
    {
        for (auto& obj : needUpdate_)
            octree_->InsertUpdate(obj);
        needUpdate_.clear();
        FrustumOctreeQuery query(visibles, *camera->GetFrustumPointer());
        octree_->Execute(query);
    }

    void Scene::Render(Camera* camera)
    {
        //if (IsReady() && camera)
		if (camera)
        {
            Graphics::this_->SetScene(this);
            Graphics::this_->SetCamera(camera);
            camera->Render();
        }
    }

    void Scene::NeedUpdate(SceneNode* obj)
    {
        needUpdate_.insert(obj);
    }

    void Scene::SavePhysics(pugi::xml_node& node) const
    {
        pugi::xml_node child = node.append_child("Physics");
        {
            std::stringstream ss;
            ss << physicsWorld_->GetGravity();
            child.append_attribute("gravity") = ss.str().c_str();
        }
    }

    void Scene::LoadPhysics(const pugi::xml_node& node)
    {
        pugi::xml_node child = node.child("Physics");
        Vertex3 gravity = GetVertex3(child.attribute("gravity").as_string());
        physicsWorld_->SetGravity(gravity);
    }

    void Scene::SaveAnimations(pugi::xml_node& node) const
    {
        pugi::xml_node child = node.append_child("Animations");
        for (auto& obj : animations_.GetConstObjs())
            obj->Save(child);
    }

    void Scene::LoadAnimations(const pugi::xml_node& node)
    {
        pugi::xml_node childAnimations = node.child("Animations");
        if (childAnimations)
        {
            pugi::xml_node child = childAnimations.child("Animation");
            while (child)
            {
                std::string name = child.attribute("name").as_string();
                if (!HasAnimation(name))
                {
                    PAnimation animation = GetOrCreateAnimation(name);
                    animation->SetScene(std::dynamic_pointer_cast<Scene>(shared_from_this()));
                    animation->Load(child);
                }
                child = child.next_sibling("Animation");
            }
        }
    }

    void Scene::SaveSkeletons(pugi::xml_node& node) const
    {
        const std::vector<PMesh>& meshes = app_.GetMeshes();
        if (meshes.size())
        {
            pugi::xml_node child = node.append_child("Skeletons");
            for (auto& obj : meshes)
            {
                PSkeleton skeleton = obj->GetSkeleton();
                if (skeleton)
                    skeleton->Save(child);
            }
        }
    }

    void Scene::LoadSkeletons(const pugi::xml_node& node)
    {
        pugi::xml_node childSkeletons = node.child("Skeletons");
        if (childSkeletons)
        {
            pugi::xml_node child = childSkeletons.child("Skeleton");
            while (child)
            {
                std::string meshName = child.attribute("meshName").as_string();
                PModelMesh mesh = std::dynamic_pointer_cast<ModelMesh>(app_.GetMesh(meshName));
                CHECK_CONDITION(mesh, __FILE__, __LINE__);
                if (!mesh->GetSkeleton())
                {
                    PSkeleton skeleton(new Skeleton(mesh));
                    skeleton->SetScene(std::dynamic_pointer_cast<Scene>(shared_from_this()));
                    skeleton->Load(child);
                    mesh->SetSkeleton(skeleton);
                }
                child = child.next_sibling("Skeleton");
            }
        }
    }

    void Scene::Load(const pugi::xml_node& node)
    {
        pugi::xml_node sceneNode = node.child("SceneNode");
        CHECK_ASSERT(sceneNode, __FILE__, __LINE__);
        SceneNode::Load(sceneNode);
        LoadAnimations(node);
        LoadSkeletons(node);
        LoadPhysics(node);
    }

    void Scene::Save(pugi::xml_node& node) const
    {
        pugi::xml_node scene = node.append_child("Scene");
        pugi::xml_node sceneNode = scene.append_child("SceneNode");
        SceneNode::Save(sceneNode);
        SaveAnimations(scene);
        SaveSkeletons(scene);
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

    PAnimation Scene::GetOrCreateAnimation(const std::string& name)
    {
        return animations_.GetOrCreate(name);
    }

    std::vector<PAnimation> Scene::GetAnimationsFor(PNode node) const
    {
        auto& animations = animations_.GetConstObjs();
        std::vector<PAnimation> result;
        for (auto& animation : animations)
        {
            auto& tracks = animation->GetTracks();
            for (auto& track : tracks)
            {
                auto trackNode = track.node_.lock();
                if (trackNode == node)
                {
                    result.push_back(animation);
                    break;
                }
            }
        }
        return result;
    }

    bool Scene::HasAnimation(const std::string& name) const
    {
        return animations_.Has(name);
    }

    bool Scene::PlayAnimation(const std::string& name, bool looped)
    {
        PAnimation animation = animations_.Get(name);
        if (animation)
        {
            PlayAnimation(animation, looped);
            return true;
        }
        return false;
    }

    void Scene::PlayAnimation(const PAnimation& animation, bool looped)
    {
        std::string name = animation->GetName();

        PAnimationState animationState;
        auto it = animationStateMap_.find(name);
        if (it != animationStateMap_.end())
            animationState = it->second;
        else
        {
            animationState = PAnimationState(new AnimationState(animation));
            animationStateMap_.insert(AnimationStateMap::value_type(name, animationState));
        }
        animationState->SetLooped(looped);
    }

    void Scene::UpdateAnimations(float deltaTime)
    {
        for (auto& obj : animationStateMap_)
        {
            obj.second->AddTime(deltaTime);
            obj.second->Update();
        }
    }

    void Scene::UpdateParticleSystems(float deltaTime)
    {
        for (auto& obj : particleSystems_)
        {
            auto ps = obj.lock();
            if (ps)
                ps->Update(deltaTime);
        }
    }

    bool Scene::SetAnimationSpeed(const std::string& name, float speed)
    {
        PAnimationState animationState;
        auto it = animationStateMap_.find(name);
        if (it != animationStateMap_.end())
            animationState = it->second;
        else
            return false;

        animationState->SetSpeed(speed);
        return true;
    }

    bool Scene::HasLight(PLight light) const
    {
        auto itType = lights_.find(light->GetType());
        if (itType != lights_.end())
        {
            auto& lights = itType->second;
            auto it = std::find_if(lights.begin(), lights.end(), [&](PWeakLight obj)
            {
                auto p = obj.lock();
                return p == light;
            });

            return it != lights.end();
        }

        return false;
    }

    void Scene::AddLight(PLight light)
    {
        if (!HasLight(light))
            lights_[light->GetType()].push_back(light);
    }

    const std::vector<PWeakLight>& Scene::GetLights(LightType type) const
    {
        return lights_[type];
    }

    void Scene::ChangeLightType(PLight light, LightType fromType)
    {
        auto itType = lights_.find(fromType);
        if (itType != lights_.end())
        {
            auto& lights = itType->second;
            auto it = std::find_if(lights.begin(), lights.end(), [&](PWeakLight obj)
            {
                auto p = obj.lock();
                return p == light;
            });

            CHECK_CONDITION(it != lights.end(), __FILE__, __LINE__);
            lights.erase(it);
            AddLight(light);
        }
    }

    void Scene::AddCamera(PCamera camera)
    {
        cameras_.push_back(camera);
    }

    const std::vector<PWeakCamera>& Scene::GetCameras() const
    {
        return cameras_;
    }

    void Scene::AddParticleSystem(PParticleSystem ps)
    {
        particleSystems_.push_back(ps);
    }

    void Scene::UpdateOctree(SceneNode* node)
    {
        octree_->InsertUpdate(node);
    }

    void Scene::RemoveFromOctree(SceneNode* node)
    {
        needUpdate_.erase(node);
        octree_->Remove(node);
    }

    SceneNode* Scene::GetClosestNode(float screenX, float screenY) const
    {
        Ray ray = Camera::GetRay(screenX, screenY);
        RayNodeResult closest{ 0, nullptr };
        GetClosestRayNodeIntersection(ray, closest);
        return closest.node_;
    }
}
