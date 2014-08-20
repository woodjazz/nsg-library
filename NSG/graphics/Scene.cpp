#include "Scene.h"
#include "SceneNode.h"
#include "Behavior.h"
#include "Light.h"
#include "Context.h"

namespace NSG
{
    Scene::Scene()
        : ambient_(0.3f, 0.3f, 0.3f, 1)
    {
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

    void Scene::Add(PSceneNode node)
    {
        Light* light = dynamic_cast<Light*>(node.get());
        
        if (light)
            lights_.push_back(light);

        nodes_.push_back(node);
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

    void Scene::Render()
    {
        Context::this_->SetScene(this);

        for (auto& obj : nodes_)
        {
            Behavior* behavior = obj->GetBehavior();
            if(behavior)
                behavior->Render();
        }
    }

    const Scene::Lights& Scene::GetLights()
    {
        return lights_;
    }
}
