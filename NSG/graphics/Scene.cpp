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
#include "pugixml.hpp"
#include <algorithm>
#include <functional>
#include <thread>

namespace NSG
{
    static const char* SCENENODE_ROOT_NAME = "SceneRootNode";

    Scene::Scene()
        : SceneNode(SCENENODE_ROOT_NAME, this),
          ambient_(0.3f, 0.3f, 0.3f, 1),
          octree_(new Octree),
          app_(*App::this_),
          started_(false)
    {
        //octree_->InsertUpdate(this);
    }

    Scene::~Scene()
    {
        cameras_.clear();
        lights_.clear();
        ClearAllChildren();
        octree_ = nullptr;
    }

    void Scene::Load(PResource resource)
    {
        SetResource(resource);
        while (!IsReady())
            std::this_thread::sleep_for(Milliseconds(10));
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
        PCamera obj(new Camera(name, this));
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

    PSceneNode Scene::CreateSceneNodeFrom(PResource resource, const std::string& name)
    {
        PSceneNode obj(CreateSceneNode(name));
        obj->Load(resource);
        octree_->InsertUpdate(obj.get());
        AddChild(obj);
        return obj;
    }

    PLight Scene::CreateLight(const std::string& name)
    {
        PLight obj(new Light(name, this));
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
        for (auto& obj : children_)
            obj->Update();
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
    }

    void Scene::Load(const pugi::xml_document& doc, const CachedData& data)
    {
        std::stringstream query;
        query << "/Scene/SceneNode[@name ='" << SCENENODE_ROOT_NAME << "']";
        pugi::xpath_node xpathNode = doc.select_single_node(query.str().c_str());
        pugi::xml_node child = xpathNode.node();
        LoadNode(child, data);
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
}
