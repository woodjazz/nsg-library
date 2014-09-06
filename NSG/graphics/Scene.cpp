#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "Behavior.h"
#include "Light.h"
#include "Context.h"
#include "Octree.h"
#include "OctreeQuery.h"
#include <algorithm>
#include <functional>

namespace NSG
{
    Scene::Scene()
        : ambient_(0.3f, 0.3f, 0.3f, 1),
          octree_(new Octree)
    {
        Context::this_->SetScene(this);
    }

    Scene::~Scene()
    {
        cameras_.clear();
        lights_.clear();
        nodes_.clear();
        Context::this_->SetScene(nullptr);
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
        cameras_.push_back(obj);
        nodes_.push_back(obj);
        return obj;
    }

    PSceneNode Scene::CreateSceneNode(const std::string& name)
    {
        PSceneNode obj(new SceneNode(name, this));
        nodes_.push_back(obj);
        return obj;
    }

    PSceneNode Scene::CreateSceneNodeFrom(PResource resource, const std::string& name)
    {
        PSceneNode obj(new SceneNode(resource, name, this));
        nodes_.push_back(obj);
        return obj;
    }

    PLight Scene::CreateLight(const std::string& name)
    {
        PLight obj(new Light(name, this));
        AddLight(obj);
        return obj;
    }

    void Scene::AddLight(PLight light)
    {
        lights_.push_back(light);
        nodes_.push_back(light);
    }

    void Scene::Start()
    {
        for (auto& obj : nodes_)
            obj->Start();
    }

    void Scene::Update()
    {
        for (auto& obj : nodes_)
            obj->Update();
    }

    void Scene::GetVisibleNodes(Camera* camera, std::vector<const SceneNode*>& visibles)
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
                if (obj.mesh_ != usedMesh || !obj.mesh_)
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
        Camera* camera = Camera::GetActiveCamera();

        if (camera)
        {
            std::vector<const SceneNode*> visibles;
            GetVisibleNodes(camera, visibles);
            AppStatistics::this_->SetNodes(nodes_.size(), visibles.size());
            std::vector<Batch> batches;
            GenerateBatches(visibles, batches);
            for (auto& batch : batches)
            {
                for (auto& node : batch.nodes_)
                {
                    ((SceneNode*)node)->Render();
                }
            }
        }
    }

    void Scene::NeedUpdate(SceneNode* obj)
    {
        needUpdate_.insert(obj);
    }
}
