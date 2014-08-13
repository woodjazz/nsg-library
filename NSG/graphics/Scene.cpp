#include "Scene.h"

namespace NSG
{
	template <> Scene* Singleton<Scene>::this_ = nullptr;

	Scene::Scene()
	: ambient_(0.3f,0.3f,0.3f,1)
	{
	}
	
	Scene::~Scene()
	{
		Scene::this_ = nullptr;
	}

	void Scene::SetAmbientColor(Color ambient)
	{
		if(ambient_ != ambient)
		{
			ambient_ = ambient;
			SetUniformsNeedUpdate();
		}
	}
}
