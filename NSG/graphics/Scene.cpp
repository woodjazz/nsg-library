#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Light.h"
#include "Behavior.h"
#include "Light.h"
#include "Context.h"
#include "Octree.h"
#include "OctreeQuery.h"

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

    void Scene::Render()
    {
        Camera* camera = Camera::GetActiveCamera();
        
        if(camera)
        {
            std::vector<const SceneNode*> visibles;
            GetVisibleNodes(camera, visibles);
            AppStatistics::this_->SetNodes(nodes_.size(), visibles.size());
			for (auto& obj : visibles)
                ((SceneNode*)obj)->Render();
        }
    }

    void Scene::NeedUpdate(SceneNode* obj)
    {
        needUpdate_.insert(obj);
    }
}
