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
          shadowPass_(std::make_shared<Pass>()),
          shadowCamera_(std::make_shared<ShadowCamera>("ShadowCamera"))
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

    void Renderer::GetLighted(std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& result) const
    {
        CHECK_ASSERT(result.empty(), __FILE__, __LINE__);

        for (auto& node : nodes)
        {
            auto material = node->GetMaterial();
            if (material && material->IsLighted())
                result.push_back(node);
        }
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

    void Renderer::Generate2DShadowMap(const Light* light, std::vector<SceneNode*>& shadowCasters)
    {
        auto frameBuffer = Graphics::this_->GetFrameBuffer();
        auto shadowFrameBuffer = light->GetShadowFrameBuffer();
        SetShadowFrameBufferSize(shadowFrameBuffer);
        if (shadowFrameBuffer->IsReady())
        {
            graphics_->SetFrameBuffer(shadowFrameBuffer);
            auto lastCamera = Graphics::this_->SetCamera(shadowCamera_.get());
            graphics_->ClearBuffers(true, true, false);
            std::vector<PBatch> batches;
            GenerateBatches(shadowCasters, batches);
            for (auto& batch : batches)
                if (batch->GetMaterial()->CastShadow())
                    Draw(batch.get(), shadowPass_.get(), light);
            Graphics::this_->SetCamera(lastCamera);
            graphics_->SetFrameBuffer(frameBuffer);
        }
    }

    void Renderer::GenerateShadowMapCubeFace(const Light* light, const std::vector<SceneNode*>& shadowCasters)
    {
        auto lastCamera = Graphics::this_->SetCamera(shadowCamera_.get());
        std::vector<SceneNode*> visiblesFromLightFace;
        shadowCamera_->GetVisiblesFromCurrentFace(shadowCasters, visiblesFromLightFace);
        std::vector<PBatch> batches;
        GenerateBatches(visiblesFromLightFace, batches);
        graphics_->ClearBuffers(true, true, false);
        for (auto& batch : batches)
            if (batch->GetMaterial()->CastShadow())
                Draw(batch.get(), shadowPass_.get(), light);
        Graphics::this_->SetCamera(lastCamera);
    }

    void Renderer::SetShadowFrameBufferSize(FrameBuffer* frameBuffer)
    {
        #ifdef IS_TARGET_MOBILE
        auto maxSize = std::max(window_->GetWidth(), window_->GetHeight());
        static const int MAX_SIZE = 1024;
        maxSize = std::min(MAX_SIZE, maxSize);
        #else
        auto maxSize = std::max(window_->GetWidth(), window_->GetHeight());
        #endif
		frameBuffer->SetSize(maxSize, maxSize);
    }

    void Renderer::GenerateCubeShadowMap(const Light* light, std::vector<SceneNode*>& shadowCasters)
    {
        auto frameBuffer = Graphics::this_->GetFrameBuffer();
        auto shadowFrameBuffer = light->GetShadowFrameBuffer();
        SetShadowFrameBufferSize(shadowFrameBuffer);
        if (shadowFrameBuffer->IsReady())
        {
            for (unsigned i = 0; i < (unsigned)CubeMapFace::MAX_CUBEMAP_FACES; i++)
            {
                TextureTarget face = (TextureTarget)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
                shadowCamera_->SetCurrentCubeShadowMapFace(face);

                auto camFrustum = camera_->GetFrustum();
                auto intersection = camFrustum->IsPointInside(light->GetGlobalPosition());
                bool genShadowMap = Intersection::OUTSIDE != intersection ||
                                    Intersection::OUTSIDE != camFrustum->IsInside(BoundingBox(*shadowCamera_->GetFrustum()));
                if (genShadowMap)
                {
                    graphics_->SetFrameBuffer(shadowFrameBuffer, face);
                    GenerateShadowMapCubeFace(light, shadowCasters);
                }
            }
            
            graphics_->SetFrameBuffer(frameBuffer);
        }
    }

    void Renderer::GenerateShadowMap(const Light* light, const std::vector<SceneNode*>& drawables)
    {
        std::vector<SceneNode*> shadowCasters;
        shadowCamera_->Setup(light, window_, camera_);
        if (shadowCamera_->GetVisibles(drawables, shadowCasters))
        {
            // Generate shadow maps
            if (light->GetType() == LightType::POINT)
                GenerateCubeShadowMap(light, shadowCasters);
			else if (LightType::DIRECTIONAL == light->GetType())
				Generate2DShadowMap(light, shadowCasters);
            else
            {
                auto intersection = camera_->GetFrustum()->IsPointInside(light->GetGlobalPosition());
                if (Intersection::OUTSIDE != intersection)
                    Generate2DShadowMap(light, shadowCasters);
                else if (Intersection::OUTSIDE != camera_->GetFrustum()->IsInside(BoundingBox(*shadowCamera_->GetFrustum())))
                    Generate2DShadowMap(light, shadowCasters);
            }
            
        }
    }

    void Renderer::AmbientPass(std::vector<SceneNode*>& nodes)
    {
        std::vector<PBatch> batches;
        GenerateBatches(nodes, batches);
        for (auto& batch : batches)
            Draw(batch.get(), ambientPass_.get(), nullptr);
    }

    void Renderer::GenerateShadowMaps()
    {
        auto drawables = scene_->GetDrawables();
        auto lights = scene_->GetLights();
        for (auto light : lights)
            if (light->DoShadows())
                GenerateShadowMap(light, drawables);
    }

    void Renderer::LitPass(std::vector<SceneNode*>& nodes)
    {
        auto lights = scene_->GetLights();
        for (auto light : lights)
        {
            if(light->GetOnlyShadow())
                continue;
			shadowCamera_->Setup(light, window_, camera_);
            std::vector<SceneNode*> litNodes;
            GetLighted(nodes, litNodes);
            std::vector<PBatch> batches;
            GenerateBatches(litNodes, batches);
            for (auto& batch : batches)
                Draw(batch.get(), lightPass_.get(), light);
        }
    }

    void Renderer::TransparentPass(std::vector<SceneNode*>& transparent)
    {
        if (transparent.empty())
            return;
        // Transparent nodes cannot be batched
        SortBackToFront(transparent);
        for (auto& node : transparent)
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
        graphics_->SetClearColor(scene->GetHorizonColor());
        camera_ = scene->GetMainCamera().get();
        graphics_->SetCamera(camera_);
        std::vector<SceneNode*> visibles;
        scene->GetVisibleNodes(camera_, visibles);
        if (!visibles.empty())
        {
            std::vector<SceneNode*> transparent;
            ExtractTransparent(visibles, transparent);
            SortFrontToBack(visibles);
            AmbientPass(visibles);
            GenerateShadowMaps();
            LitPass(visibles);
            TransparentPass(transparent);
        }
    }
}