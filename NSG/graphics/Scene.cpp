#include "Scene.h"
#include "SceneNode.h"
#include "Behavior.h"
#include "Light.h"

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
        for (auto& obj : nodes_)
        {
            Behavior* behavior = obj->GetBehavior();
            if(behavior)
                behavior->Render();
        }
    }

    void Scene::OnMouseMove(float x, float y)
    {
        for (auto& obj : nodes_)
        {
            Behavior* behavior = obj->GetBehavior();
            if(behavior)
                behavior->OnMouseMove(x, y);
        }
    }

    void Scene::OnMouseDown(float x, float y)
    {
        for (auto& obj : nodes_)
        {
            Behavior* behavior = obj->GetBehavior();
            if(behavior)
                behavior->OnMouseDown(x, y);
        }
    }

    void Scene::OnMouseUp()
    {
        for (auto& obj : nodes_)
        {
            Behavior* behavior = obj->GetBehavior();
            if(behavior)
                behavior->OnMouseUp();
        }
    }

    void Scene::OnKey(int key, int action, int modifier)
    {
        for (auto& obj : nodes_)
        {
            Behavior* behavior = obj->GetBehavior();
            if(behavior)
                behavior->OnKey(key, action, modifier);
        }
    }

    void Scene::OnChar(unsigned int character)
    {
        for (auto& obj : nodes_)
        {
            Behavior* behavior = obj->GetBehavior();
            if(behavior)
                behavior->OnChar(character);
        }
    }

    const Scene::Lights& Scene::GetLights()
    {
        return lights_;
    }
}
