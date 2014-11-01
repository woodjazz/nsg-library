#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "Behavior.h"
#include "Light.h"
#include "Context.h"
#include "Octree.h"
#include "OctreeQuery.h"
#include "Graphics.h"
#include "Constants.h"
#include "Material.h"
#include "Mesh.h"
#include "App.h"
#include "Skeleton.h"
#include "Animation.h"
#include "AnimationState.h"
#include "PhysicsWorld.h"
#include "pugixml.hpp"
#include <algorithm>
#include <functional>
#include <thread>

namespace NSG
{
    static const char* SCENENODE_ROOT_NAME = "SceneRootNode";

    Scene::Scene()
        : SceneNode(SCENENODE_ROOT_NAME),
          ambient_(0.3f, 0.3f, 0.3f, 1),
          octree_(new Octree),
          app_(*App::this_),
          started_(false),
          physicsWorld_(new PhysicsWorld)
    {
        //octree_->InsertUpdate(this);
    }

    Scene::~Scene()
    {
        octree_ = nullptr;
        cameras_.clear();
        lights_.clear();
        ClearAllChildren();

    }

    void Scene::Load(PResource resource)
    {
        SetResource(resource);
    }

    void Scene::SetAmbientColor(Color ambient)
    {
        if (ambient_ != ambient)
        {
            ambient_ = ambient;
            SetUniformsNeedUpdate();
        }
    }

    PCamera Scene::CreateCamera(const std::string& name)
    {
        PCamera obj(new Camera(name));
        octree_->InsertUpdate(obj.get());
        cameras_.push_back(obj);
        AddChild(obj);
        return obj;
    }

    void Scene::AddCamera(PCamera camera)
    {
        octree_->InsertUpdate(camera.get());
        cameras_.push_back(camera);
        AddChild(camera);
    }

    PSceneNode Scene::CreateSceneNode(const std::string& name)
    {
        PSceneNode obj = CreateChild(name);
        octree_->InsertUpdate(obj.get());
        return obj;
    }

    PLight Scene::CreateLight(const std::string& name)
    {
        PLight obj(new Light(name));
        AddLight(obj);
        return obj;
    }

    PLight Scene::CreatePointLight(const std::string& name)
    {
        PLight light = CreateLight(name);
        light->SetType(Light::POINT);
        return light;
    }

    PLight Scene::CreateDirectionalLight(const std::string& name)
    {
        PLight light = CreateLight(name);
        light->SetType(Light::DIRECTIONAL);
        return light;
    }

    PLight Scene::CreateSpotLight(const std::string& name)
    {
        PLight light = CreateLight(name);
        light->SetType(Light::SPOT);
        return light;
    }


    void Scene::AddLight(PLight light)
    {
        octree_->InsertUpdate(light.get());
        lights_.push_back(light);
        AddChild(light);
    }

    void Scene::Start()
    {
        if (!started_)
        {
            for (auto& obj : children_)
                obj->Start();

            started_ = true;
        }
    }

    void Scene::Update()
    {
		float dt = app_.GetDeltaTime();
        
        physicsWorld_->StepSimulation(dt);

        for (auto& obj : children_)
            obj->Update();

        UpdateAnimations();
    }

    void Scene::ViewChanged(int width, int height)
    {
        for (auto& obj : children_)
            obj->ViewChanged(width, height);
    }

    void Scene::OnMouseMove(float x, float y)
    {
        for (auto& obj : children_)
            obj->OnMouseMove(x, y);
    }

    void Scene::OnMouseDown(int button, float x, float y)
    {
        for (auto& obj : children_)
            obj->OnMouseDown(button, x, y);
    }

    void Scene::OnMouseWheel(float x, float y)
    {
        for (auto& obj : children_)
            obj->OnMouseWheel(x, y);
    }

    void Scene::OnMouseUp(int button, float x, float y)
    {
        for (auto& obj : children_)
            obj->OnMouseUp(button, x, y);
    }

	void Scene::OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
    {
        for (auto& obj : children_)
			obj->OnMultiGesture(timestamp, x, y, dTheta, dDist, numFingers);
    }

    void Scene::OnKey(int key, int action, int modifier)
    {
        for (auto& obj : children_)
            obj->OnKey(key, action, modifier);
    }

    void Scene::OnChar(unsigned int character)
    {
        for (auto& obj : children_)
            obj->OnChar(character);
    }

    bool Scene::GetFastRayNodesIntersection(const Ray& ray, std::vector<const SceneNode*>& nodes) const
    {
        RayOctreeQuery query(nodes, ray);
        octree_->Execute(query);
        return !nodes.empty();
    }

    bool Scene::GetPreciseRayNodesIntersection(const Ray& ray, std::vector<RayNodeResult>& result) const
    {
        std::vector<const SceneNode*> tmpNodes;
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

    bool Scene::GetClosestRayNodeIntersection(const Ray& ray, RayNodeResult& closest)
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


    void Scene::GetVisibleNodes(const Camera* camera, std::vector<const SceneNode*>& visibles) const
    {
        for (auto& obj : needUpdate_)
            octree_->InsertUpdate(obj);

        needUpdate_.clear();

        FrustumOctreeQuery query(visibles, *camera->GetFrustumPointer());
        octree_->Execute(query);
    }

    void Scene::GenerateBatches(std::vector<const SceneNode*>& visibles, std::vector<Batch>& batches)
    {
        struct MeshNode
        {
            PMesh mesh_;
            const SceneNode* node_;
        };

        struct MaterialData
        {
            PMaterial material_;
            std::vector<MeshNode> data_;
        };

        std::sort(visibles.begin(), visibles.end(), [](const SceneNode * a, const SceneNode * b) -> bool
        {
            return a->GetMaterial().get() < b->GetMaterial().get();
        });

        std::vector<MaterialData> materials;
        PMaterial usedMaterial;
        for (auto& node : visibles)
        {
            PMaterial material = node->GetMaterial();
            PMesh mesh = node->GetMesh();

            if (usedMaterial != material || !material)
            {
                usedMaterial = material;
                MaterialData materialData;
                materialData.material_ = material;
                materialData.data_.push_back({mesh, node});
                if (!materials.empty())
                {
                    MaterialData& lastMaterialData = materials.back();
                    std::sort(lastMaterialData.data_.begin(), lastMaterialData.data_.end(), [](const MeshNode & a, const MeshNode & b) -> bool
                    {
                        return a.mesh_.get() < b.mesh_.get();
                    });
                }
                materials.push_back(materialData);
            }
            else
            {
                MaterialData& lastMaterial = materials.back();
                lastMaterial.data_.push_back({mesh, node});
            }
        }

        for (auto& material : materials)
        {
            PMesh usedMesh;
            for (auto& obj : material.data_)
            {
                bool limitReached = batches.size() && batches.back().nodes_.size() >= MAX_NODES_IN_BATCH;
                if (obj.mesh_ != usedMesh || !obj.mesh_ || limitReached)
                {
                    usedMesh = obj.mesh_;
                    Batch batch;
                    batch.material_ = material.material_;
                    batch.mesh_ = usedMesh;
                    batch.nodes_.push_back(obj.node_);
                    batches.push_back(batch);
                }
                else
                {
                    Batch& lastBatch = batches.back();
                    lastBatch.nodes_.push_back(obj.node_);
                }
            }
        }
    }

    void Scene::Render()
    {
        if (IsReady())
        {
            Camera* camera = Camera::GetActiveCamera();

            if (camera)
            {
                camera->BeginRender();
                std::vector<const SceneNode*> visibles;
                GetVisibleNodes(camera, visibles);
                AppStatistics::this_->SetNodes(children_.size(), visibles.size());
                std::vector<Batch> batches;
                GenerateBatches(visibles, batches);
                for (auto& batch : batches)
                    Graphics::this_->Render(batch);
                camera->EndRender();
            }
        }
    }

    void Scene::NeedUpdate(SceneNode* obj)
    {
        needUpdate_.insert(obj);
    }

    Scene::Lights Scene::GetLights(Light::Type type) const
    {
        Lights lights;

        for (auto& light : lights_)
        {
            if (light->GetType() == type)
                lights.push_back(light);
        }

        return lights;
    }

    void Scene::SaveMeshes(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Meshes");
        auto meshes = app_.GetMeshes();
        for (auto& obj : meshes)
            obj->Save(child);
    }

    void Scene::SaveMaterials(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Materials");
        auto materials = app_.GetMaterials();
        for (auto& obj : materials)
            obj->Save(child);
    }

    void Scene::Save(pugi::xml_document& doc)
    {
        pugi::xml_node child = doc.append_child("Scene");
        SaveMeshes(child);
        SaveMaterials(child);
        SceneNode::Save(child);
        SaveAnimations(child);
        SaveSkeletons(child);
    }

    void Scene::SaveAnimations(pugi::xml_node& node)
    {
        pugi::xml_node child = node.append_child("Animations");
        for (auto& obj : animationMap_)
            obj.second->Save(child);
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
                PAnimation animation = CreateAnimation(name);
                animation->Load(child);
                child = child.next_sibling("Animation");
            }
        }
    }

    void Scene::SaveSkeletons(pugi::xml_node& node)
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
            const std::vector<PMesh>& meshes = app_.GetMeshes();
            pugi::xml_node child = childSkeletons.child("Skeleton");
            while (child)
            {
                std::string meshName = child.attribute("meshName").as_string();
                PMesh mesh;
                for (auto& obj : meshes)
                {
                    if (obj->GetName() == meshName)
                    {
                        mesh = obj;
                        break;
                    }
                }
                CHECK_CONDITION(mesh, __FILE__, __LINE__);
                PSkeleton skeleton(new Skeleton(mesh));
                skeleton->Load(child);
                mesh->SetSkeleton(skeleton);
                child = child.next_sibling("Skeleton");
            }
        }
    }

    void Scene::Load(const pugi::xml_document& doc, const CachedData& data)
    {
        //std::stringstream query;
        //query << "/Scene/SceneNode[@name ='" << SCENENODE_ROOT_NAME << "']";
        //pugi::xpath_node xpathNode = doc.select_single_node(query.str().c_str());
        pugi::xml_node sceneChild = doc.child("Scene");
        if (sceneChild)
        {
            pugi::xml_node sceneNodeChild = sceneChild.child("SceneNode");
            SetName(sceneNodeChild.attribute("name").as_string());
            if (sceneNodeChild)
                LoadNode(sceneNodeChild, data);
            LoadAnimations(sceneChild);
            LoadSkeletons(sceneChild);
        }
    }

    bool Scene::GetVisibleBoundingBox(const Camera* camera, BoundingBox& bb) const
    {
        std::vector<const SceneNode*> visibles;
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

    PAnimation Scene::CreateAnimation(const std::string& name)
    {
        CHECK_ASSERT(!name.empty(), __FILE__, __LINE__);
        PAnimation animation(new Animation);
        CHECK_CONDITION(animationMap_.insert(AnimationMap::value_type(name, animation)).second, __FILE__, __LINE__);
        animation->SetName(name);
        return animation;
    }

    bool Scene::HasAnimation(const std::string& name) const
    {
        return animationMap_.find(name) != animationMap_.end();
    }

    bool Scene::PlayAnimation(const std::string& name, bool looped)
    {
        auto it = animationMap_.find(name);
        if (it == animationMap_.end())
            return false;
        PAnimation animation = it->second;
        return PlayAnimation(animation, looped);
    }

    bool Scene::PlayAnimation(const PAnimation& animation, bool looped)
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
        return true;
    }

    void Scene::UpdateAnimations()
    {
        float deltaTime = app_.GetDeltaTime();
        for (auto& obj : animationStateMap_)
        {
            obj.second->AddTime(deltaTime);
            obj.second->Update();
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

}
