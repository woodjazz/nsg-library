/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "Renderer.h"
#include "Graphics.h"
#include "Scene.h"
#include "Camera.h"
#include "Material.h"
#include "SceneNode.h"
#include "Batch.h"
#include "Pass.h"
#include "Node.h"

namespace NSG
{
    Renderer* Renderer::this_ = nullptr;

    Renderer::Renderer(Graphics* graphics)
        : graphics_(graphics),
          window_(nullptr),
          scene_(nullptr),
          camera_(nullptr)
    {
        CHECK_ASSERT(!Renderer::this_, __FILE__, __LINE__);
        Renderer::this_ = this;
    }

    Renderer::~Renderer()
    {
        CHECK_ASSERT(Renderer::this_, __FILE__, __LINE__);
        Renderer::this_ = nullptr;
    }

    void Renderer::ExtractTransparent(std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& transparent) const
    {
        CHECK_ASSERT(transparent.empty(), __FILE__, __LINE__);

        for (auto& node : nodes)
        {
            auto material = node->GetMaterial();
            if (material && material->IsTransparent())
                transparent.push_back(node);
        }

        // remove tranparent from nodes
        nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
        [&](SceneNode * node) {return transparent.end() != std::find(transparent.begin(), transparent.end(), node); }),
        nodes.end());
    }

    void Renderer::SortBackToFront(std::vector<SceneNode*>& nodes) const
    {
        Vector3 cameraPos;
        if (camera_)
            cameraPos = camera_->GetGlobalPosition();
        std::sort(nodes.begin(), nodes.end(), [&](const SceneNode * a, const SceneNode * b) -> bool
        {
            auto da = glm::distance2(a->GetGlobalPosition(), cameraPos);
            auto db = glm::distance2(b->GetGlobalPosition(), cameraPos);
            return db < da;
        });
    }

    void Renderer::SortFrontToBack(std::vector<SceneNode*>& nodes) const
    {
        Vector3 cameraPos;
        if (camera_)
            cameraPos = camera_->GetGlobalPosition();
        std::sort(nodes.begin(), nodes.end(), [&](const SceneNode * a, const SceneNode * b) -> bool
        {
            auto da = glm::distance2(a->GetGlobalPosition(), cameraPos);
            auto db = glm::distance2(b->GetGlobalPosition(), cameraPos);
            return da < db;
        });
    }


    void Renderer::GenerateBatches(std::vector<SceneNode*>& visibles, std::vector<PBatch>& batches)
    {
        batches.clear();

        struct MeshNode
        {
            PMesh mesh_;
            SceneNode* node_;
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
            if (!material) continue;
            auto mesh = node->GetMesh();
            if (usedMaterial != material)
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
                bool limitReached = batches.size() && batches.back()->GetNodes().size() >= MAX_NODES_IN_BATCH;
                if (obj.mesh_ != usedMesh || !obj.mesh_ || limitReached)
                {
                    usedMesh = obj.mesh_;
                    auto batch(std::make_shared<Batch>(material.material_.get(), usedMesh.get()));
                    batch->Add(obj.node_);
                    batches.push_back(batch);
                }
                else
                {
                    auto& lastBatch = batches.back();
                    lastBatch->Add(obj.node_);
                }
            }
        }
    }

    void Renderer::Draw(Batch* batch, Pass* pass, Light* light)
    {
        graphics_->SetMesh(batch->GetMesh());
        if (batch->AllowInstancing())
        {
            graphics_->SetNode(nullptr);
            graphics_->SetupPass(pass, batch->GetMaterial(), light);
            graphics_->DrawInstancedActiveMesh(pass, *batch);
        }
        else
        {
            auto& nodes = batch->GetNodes();
            for (auto& node : nodes)
            {
                SceneNode* sn = (SceneNode*)node;
                graphics_->SetNode(sn);
                graphics_->SetupPass(pass, batch->GetMaterial(), light);
                graphics_->DrawActiveMesh(pass);
            }
        }
    }


    void Renderer::RenderVisibleSceneNodes(std::vector<SceneNode*>& visibles)
    {
        std::vector<SceneNode*> transparent;
        ExtractTransparent(visibles, transparent);
        Pass pass;
        auto lights = scene_->GetLights();
        if (lights.empty())
            lights.push_back(nullptr);
        for (auto light : lights)
        {
            if (!visibles.empty())
            {
                // First draw non-transparent nodes
                SortFrontToBack(visibles);
                std::vector<PBatch> allBatches;
                GenerateBatches(visibles, allBatches);
                for (auto& batch : allBatches)
                    Draw(batch.get(), &pass, light);
            }
            pass.SetBlendMode(BLEND_MODE::ADDITIVE);
            pass.SetDepthFunc(DepthFunc::LEQUAL);
            pass.EnableDepthBuffer(false);
        }
        if (!transparent.empty())
        {
			pass.EnableDepthBuffer(false);
            pass.SetBlendMode(BLEND_MODE::ALPHA);
            // Transparent nodes cannot be batched
            SortBackToFront(transparent);
            for (auto& node : transparent)
            {
                auto sceneNode = (SceneNode*)node;
                auto material = sceneNode->GetMaterial().get();
                if (material)
                {
                    graphics_->SetNode(sceneNode);
                    graphics_->SetMesh(sceneNode->GetMesh().get());
                    graphics_->SetupPass(&pass, material, nullptr);
                    graphics_->DrawActiveMesh(&pass);
                }
            }
        }
    }

    void Renderer::Render(Window* window, Scene* scene)
    {
        window_ = window;
        scene_ = scene;
        graphics_->SetWindow(window);
        graphics_->ClearAllBuffers();
        if (scene)
        {
            graphics_->SetScene(scene);

			RenderLayer layer = RenderLayer::DEFAULT_LAYER;
            if (scene->GetDrawablesNumber(layer))
            {
                camera_ = nullptr;
                auto cameras = scene->GetCameras(layer);
                if (!cameras.empty()) camera_ = cameras[0];
                graphics_->SetCamera(camera_);
                std::vector<SceneNode*> visibles;
                scene->GetVisibleNodes(layer, camera_, visibles);
                if (!visibles.empty())
                    RenderVisibleSceneNodes(visibles);
            }
#if 0
			camera_ = scene_->GetOrthoCamera().get();
			for (int i = (int)RenderLayer::GUI_LAYER0; i < (int)RenderLayer::MAX_LAYERS; i++)
			{
				RenderLayer layer = (RenderLayer)i;
				auto cameras = scene->GetCameras(layer);
				if (!cameras.empty()) 
					camera_ = cameras[0];
				graphics_->SetCamera(camera_);
				std::vector<SceneNode*> visibles;
				scene->GetVisibleNodes(layer, camera_, visibles);
				if (!visibles.empty())
					RenderVisibleSceneNodes(visibles);
			}
#endif
        }
    }

}