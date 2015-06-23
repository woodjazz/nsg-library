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
#include "Frustum.h"
#include "ShadowCamera.h"
#include "Material.h"
#include "SceneNode.h"
#include "Batch.h"
#include "Pass.h"
#include "Node.h"
#include "Program.h"
#include "FrameBuffer.h"
#include "Window.h"

namespace NSG
{
    Renderer* Renderer::this_ = nullptr;

    Renderer::Renderer(Graphics* graphics)
        : graphics_(graphics),
          window_(nullptr),
          scene_(nullptr),
          camera_(nullptr),
          ambientPass_(std::make_shared<Pass>()),
          lightPass_(std::make_shared<Pass>()),
          transparentPass_(std::make_shared<Pass>()),
          shadowPass_(std::make_shared<Pass>())
    {

        CHECK_ASSERT(!Renderer::this_, __FILE__, __LINE__);
        Renderer::this_ = this;

        ambientPass_->SetType(PassType::AMBIENT);

        lightPass_->SetType(PassType::LIT);
        lightPass_->EnableDepthBuffer(false);
        lightPass_->SetBlendMode(BLEND_MODE::ADDITIVE);
        lightPass_->SetDepthFunc(DepthFunc::LEQUAL);

        transparentPass_->EnableDepthBuffer(false);
        transparentPass_->SetBlendMode(BLEND_MODE::ALPHA);

        shadowPass_->SetType(PassType::SHADOW);
    }

    Renderer::~Renderer()
    {
        CHECK_ASSERT(Renderer::this_, __FILE__, __LINE__);
        Renderer::this_ = nullptr;
    }

    void Renderer::ExtractTransparent()
    {
        transparent_.clear();

        for (auto& node : visibles_)
        {
            auto material = node->GetMaterial();
            if (material && material->IsTransparent())
                transparent_.push_back(node);
        }

        auto condition = [&](SceneNode * node)
        {
            return transparent_.end() != std::find(transparent_.begin(), transparent_.end(), node);
        };

        // remove tranparent from visibles_
        visibles_.erase(std::remove_if(visibles_.begin(), visibles_.end(), condition), visibles_.end());
    }

    void Renderer::GetLighted(std::vector<SceneNode*>& result) const
    {
        CHECK_ASSERT(result.empty(), __FILE__, __LINE__);

        for (auto& node : visibles_)
        {
            auto material = node->GetMaterial();
            if (material && material->IsLighted())
                result.push_back(node);
        }
    }

    void Renderer::SortTransparentBackToFront()
    {
        Vector3 cameraPos;
        if (camera_)
            cameraPos = camera_->GetGlobalPosition();
        std::sort(transparent_.begin(), transparent_.end(), [&](const SceneNode * a, const SceneNode * b) -> bool
        {
            auto da = glm::distance2(a->GetGlobalPosition(), cameraPos);
            auto db = glm::distance2(b->GetGlobalPosition(), cameraPos);
            return db < da;
        });
    }

    void Renderer::SortSolidFrontToBack()
    {
        Vector3 cameraPos;
        if (camera_)
            cameraPos = camera_->GetGlobalPosition();
        std::sort(visibles_.begin(), visibles_.end(), [&](const SceneNode * a, const SceneNode * b) -> bool
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
            if (!material)
                continue;
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

    void Renderer::DrawShadowPass(Batch* batch, const Light* light)
    {
        Draw(batch, shadowPass_.get(), light);
    }

    void Renderer::Draw(Batch* batch, const Pass* pass, const Light* light)
    {
        graphics_->SetMesh(batch->GetMesh());
        if (graphics_->SetupPass(pass, nullptr, batch->GetMaterial(), light))
        {
            if (batch->AllowInstancing())
                graphics_->DrawInstancedActiveMesh(*batch);
            else
            {
                auto& nodes = batch->GetNodes();
                for (auto& node : nodes)
                {
                    auto program = graphics_->GetProgram();
                    program->Set(node);
                    program->SetNodeVariables();
                    graphics_->DrawActiveMesh();
                }
            }
        }
    }

    void Renderer::AmbientPass()
    {
        std::vector<PBatch> batches;
        GenerateBatches(visibles_, batches);
        for (auto& batch : batches)
            Draw(batch.get(), ambientPass_.get(), nullptr);
    }

    void Renderer::GenerateShadowMaps()
    {
        auto lights = scene_->GetLights();
        for (auto light : lights)
            if (light->DoShadows())
                light->GenerateShadowMaps(camera_);
    }

    void Renderer::LitPass()
    {
        auto lights = scene_->GetLights();
        for (auto light : lights)
        {
            if (light->GetOnlyShadow())
                continue;
            std::vector<SceneNode*> litNodes;
            GetLighted(litNodes);
            std::vector<PBatch> batches;
            GenerateBatches(litNodes, batches);
            for (auto& batch : batches)
                Draw(batch.get(), lightPass_.get(), light);
        }
    }

    void Renderer::TransparentPass()
    {
        if (transparent_.empty())
            return;
        // Transparent nodes cannot be batched
        SortTransparentBackToFront();
        for (auto& node : transparent_)
        {
            auto sceneNode = (SceneNode*)node;
            auto material = sceneNode->GetMaterial().get();
            if (material)
            {
                graphics_->SetMesh(sceneNode->GetMesh().get());
                if (graphics_->SetupPass(transparentPass_.get(), sceneNode, material, nullptr))
                    graphics_->DrawActiveMesh();
            }
        }
    }

    void Renderer::Render(Window* window, Scene* scene)
    {
        window_ = window;
        scene_ = scene;
        graphics_->SetWindow(window);
        graphics_->ClearAllBuffers();
        if (!scene || scene->GetDrawablesNumber() == 0)
            return;
        camera_ = scene->GetMainCamera().get();
        graphics_->SetCamera(camera_);
        scene->GetVisibleNodes(camera_, visibles_);
        if (!visibles_.empty())
        {
            ExtractTransparent();
            SortSolidFrontToBack();
            graphics_->SetClearColor(COLOR_WHITE);
            GenerateShadowMaps();
            graphics_->SetClearColor(scene->GetHorizonColor());
            AmbientPass();
            LitPass();
            TransparentPass();
        }
    }
}