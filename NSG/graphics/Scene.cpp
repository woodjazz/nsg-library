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
    }

    void Scene::SetAmbientColor(Color ambient)
    {
        if (ambient_ != ambient)
        {
            ambient_ = ambient;
            SetUniformsNeedUpdate();
        }
    }

    PCamera Scene::CreateCamera()
    {
        PCamera obj(new Camera);
        cameras_.push_back(obj);
        nodes_.push_back(obj);
        octree_->InsertDrawable(obj.get());
        return obj;
    }

    PSceneNode Scene::CreateSceneNode()    
    {
        PSceneNode obj(new SceneNode);
        nodes_.push_back(obj);
		octree_->InsertDrawable(obj.get());
        return obj;
    }

    PLight Scene::CreateLight()
    {
        PLight obj(new Light);
        lights_.push_back(obj);
        nodes_.push_back(obj);
		octree_->InsertDrawable(obj.get());
        return obj;
    }

    void Scene::Start()
    {
        for (auto& obj : nodes_)
        {
            Behavior* behavior = obj->GetBehavior();
            if(behavior)
                behavior->Start();
        }
    }

    void Scene::Update()
    {
        for (auto& obj : nodes_)
        {
            Behavior* behavior = obj->GetBehavior();
            if(behavior)
                behavior->Update();
        }
    }

    void Scene::GetVisibleNodes(Camera* camera, std::vector<const SceneNode*>& visibles)
    {
        auto it = needUpdate_.begin();
        while(it != needUpdate_.end())
        {
            const SceneNode* node = *it;
            octree_->Update(node);
            it = needUpdate_.erase(it);
        }

        FrustumOctreeQuery query(visibles, *camera->GetFrustumPointer());
        octree_->GetDrawables(query);
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
			//for (auto& obj : nodes_)
            {
                Behavior* behavior = obj->GetBehavior();
                if(behavior)
                    behavior->Render();
            }
        }
    }

    void Scene::Update(const SceneNode* obj)
    {
        needUpdate_.insert(obj);
    }
}
