/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#pragma once
#include "Types.h"
#include "SceneNode.h"
#include "Light.h"
#include <vector>
#include <set>

namespace NSG
{
	class Scene : public SceneNode
    {
    public:
        ~Scene();
        void SetAmbientColor(Color ambient);
        const Color& GetAmbientColor() const
        {
            return ambient_;
        }
        PCamera CreateCamera(const std::string& name);
        void AddCamera(PCamera camera);
        PSceneNode CreateSceneNode(const std::string& name);
        PSceneNode CreateSceneNodeFrom(PResource resource, const std::string& name);
        PLight CreateLight(const std::string& name);
        PLight CreatePointLight(const std::string& name);
        PLight CreateDirectionalLight(const std::string& name);
		PLight CreateSpotLight(const std::string& name);
        void AddLight(PLight light);
        void Start();
        void Update();
        void Render();
        void Load(PResource resource);
        void ViewChanged(int width, int height);
        void OnMouseMove(float x, float y);
		void OnMouseDown(int button, float x, float y);
        void OnMouseWheel(float x, float y);
		void OnMouseUp(int button, float x, float y);
        void OnKey(int key, int action, int modifier);
        void OnChar(unsigned int character);

        typedef std::vector<PLight> Lights;
        const Lights& GetLights()
        {
            return lights_;
        }
        void NeedUpdate(SceneNode* obj);
        void GetVisibleNodes(Camera* camera, std::vector<const SceneNode*>& visibles);
        POctree GetOctree() const
        {
            return octree_;
        }
        Lights GetLights(Light::Type type) const;
        void Save(pugi::xml_document& doc);
		virtual void Load(const pugi::xml_document& doc, const CachedData& data) override;
        bool GetFastRayNodesIntersection(const Ray& ray, std::vector<const SceneNode*>& nodes) const;
        bool GetPreciseRayNodesIntersection(const Ray& ray, std::vector<RayNodeResult>& result) const;
        bool GetClosestRayNodeIntersection(const Ray& ray, RayNodeResult& closest);
	protected:
		Scene();
		void SaveMeshes(pugi::xml_node& node);
		void SaveMaterials(pugi::xml_node& node);
	private:
		void GenerateBatches(std::vector<const SceneNode*>& visibles, std::vector<Batch>& batches);
    private:
        Color ambient_;
        std::vector<PCamera> cameras_;
        Lights lights_;
        POctree octree_;
        std::set<SceneNode*> needUpdate_;
        App& app_;
        bool started_;
        friend class App;
    };
}