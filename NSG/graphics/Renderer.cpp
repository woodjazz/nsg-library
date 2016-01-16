/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "RenderingContext.h"
#include "Scene.h"
#include "Camera.h"
#include "Frustum.h"
#include "ShadowCamera.h"
#include "Material.h"
#include "SceneNode.h"
#include "Batch.h"
#include "InstanceBuffer.h"
#include "Pass.h"
#include "Node.h"
#include "Program.h"
#include "FrameBuffer.h"
#include "Window.h"
#include "PhysicsWorld.h"
#include "LinesMesh.h"
#include "Check.h"
#include "DebugRenderer.h"
#include "Texture.h"
#include "QuadMesh.h"

namespace NSG
{
    Renderer::Renderer()
        : graphics_(RenderingContext::GetPtr()),
          window_(nullptr),
          scene_(nullptr),
          camera_(nullptr),
          debugPhysics_(false),
          debugMaterial_(Material::Create("NSGDebugMaterial")),
          debugRenderer_(std::make_shared<DebugRenderer>()),
          context_(RendererContext::DEFAULT),
          overlaysCamera_(std::make_shared<Camera>("NSGOverlays")),
          instanceBuffer_(new InstanceBuffer())
    {
        debugMaterial_->SetSerializable(false);

        shadowPass_.SetType(PassType::SHADOW);

        defaultOpaquePass_.SetType(PassType::DEFAULT);

        litOpaquePass_.SetType(PassType::LIT);
        litOpaquePass_.EnableDepthBuffer(false);
        litOpaquePass_.SetBlendMode(BLEND_MODE::ADDITIVE);
        litOpaquePass_.SetDepthFunc(DepthFunc::LEQUAL);

        defaultTransparentPass_.SetType(PassType::DEFAULT);
        defaultTransparentPass_.EnableDepthBuffer(false);
        defaultTransparentPass_.SetBlendMode(BLEND_MODE::ALPHA);

        litTransparentPass_.SetType(PassType::LIT);
        litTransparentPass_.EnableDepthBuffer(false);
        litTransparentPass_.SetBlendMode(BLEND_MODE::ALPHA);

        debugPass_.SetType(PassType::DEFAULT);
        debugPass_.EnableDepthBuffer(false);
        debugPass_.SetDepthFunc(DepthFunc::LEQUAL);
        debugPass_.SetBlendMode(BLEND_MODE::ADDITIVE);

        filterPass_.SetType(PassType::DEFAULT);
        //filterPass_.EnableDepthBuffer(false);
        filterPass_.SetDepthFunc(DepthFunc::LEQUAL);
        filterPass_.SetBlendMode(BLEND_MODE::ALPHA);

        addPass_.EnableDepthTest(false);
        addPass_.SetBlendMode(BLEND_MODE::ADDITIVE);

        debugMaterial_->SetRenderPass(RenderPass::VERTEXCOLOR);
        debugMaterial_->SetFillMode(FillMode::WIREFRAME);

        overlaysCamera_->EnableOrtho();
        overlaysCamera_->SetNearClip(-1000);
        overlaysCamera_->SetFarClip(1000);
        overlaysCamera_->UnRegisterWindow();
    }

    Renderer::~Renderer()
    {
    }

    inline std::vector<SceneNode*> Renderer::ExtractTransparent(const std::vector<SceneNode*>& objs)
    {
        std::vector<SceneNode*> result;
        for (auto& node : objs)
        {
            auto material = node->GetMaterial();
            if (material && material->IsTransparent())
                result.push_back(node);
        }
        return result;
    }

    inline void Renderer::RemoveFrom(std::vector<SceneNode*>& from, const std::vector<SceneNode*>& objs)
    {
        auto condition = [&](SceneNode * node)
        {
            return objs.end() != std::find(objs.begin(), objs.end(), node);
        };

        from.erase(std::remove_if(from.begin(), from.end(), condition), from.end());
    }

    inline std::vector<SceneNode*> Renderer::ExtractFiltered(const std::vector<SceneNode*>& objs)
    {
        std::vector<SceneNode*> result;
        for (auto& node : objs)
        {
            if (node->HasFilter())
                result.push_back(node);
        }
        return result;
    }


    void Renderer::GetLightedBatches(const std::vector<SceneNode*>& nodes, const std::vector<Batch>& iBatches, std::vector<const Batch*>& result) const
    {
        CHECK_ASSERT(result.empty());
        for (auto& batch : iBatches)
            if (batch.GetMaterial()->IsLighted())
                result.push_back(&batch);
    }

    void Renderer::SortTransparentBackToFront(std::vector<SceneNode*>& objs)
    {
        Vector3 cameraPos;
        if (camera_)
            cameraPos = camera_->GetGlobalPosition();
        std::sort(objs.begin(), objs.end(), [&](const SceneNode * a, const SceneNode * b) -> bool
        {
            auto da = Distance2(a->GetGlobalPosition(), cameraPos);
            auto db = Distance2(b->GetGlobalPosition(), cameraPos);
            return db < da;
        });
    }

    void Renderer::SortOverlaysBackToFront(std::vector<SceneNode*>& objs)
    {
        Vector3 cameraPos(overlaysCamera_->GetGlobalPosition());
        std::sort(objs.begin(), objs.end(), [&](const SceneNode * a, const SceneNode * b) -> bool
        {
            auto da = Distance2(a->GetGlobalPosition(), cameraPos);
            auto db = Distance2(b->GetGlobalPosition(), cameraPos);
            return db < da;
        });
    }


    void Renderer::SortSolidFrontToBack(std::vector<SceneNode*>& objs)
    {
        Vector3 cameraPos;
        if (camera_)
            cameraPos = camera_->GetGlobalPosition();
        std::sort(objs.begin(), objs.end(), [&](const SceneNode * a, const SceneNode * b) -> bool
        {
            auto da = Distance2(a->GetGlobalPosition(), cameraPos);
            auto db = Distance2(b->GetGlobalPosition(), cameraPos);
            return da < db;
        });
    }


    void Renderer::GenerateBatches(std::vector<SceneNode*>& visibles, std::vector<Batch>& batches)
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
                materialData.data_.push_back({ mesh, node });
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
                lastMaterial.data_.push_back({ mesh, node });
            }
        }

        for (auto& material : materials)
        {
            PMesh usedMesh;
            for (auto& obj : material.data_)
            {
                bool limitReached = batches.size() && batches.back().GetNodes().size() >= MAX_NODES_IN_BATCH;
                if (obj.mesh_ != usedMesh || !obj.mesh_ || limitReached)
                {
                    usedMesh = obj.mesh_;
                    Batch batch(material.material_.get(), usedMesh.get());
                    batch.Add(obj.node_);
                    batches.push_back(batch);
                }
                else
                {
                    auto& lastBatch = batches.back();
                    lastBatch.Add(obj.node_);
                }
            }
        }
    }

    void Renderer::DrawShadowPass(Batch* batch, const Light* light, const ShadowCamera* camera)
    {
        Draw(batch, &shadowPass_, light, camera);
    }

    void Renderer::Draw(const Batch* batch, const Pass* pass, const Light* light, const Camera* camera)
    {
        graphics_->SetMesh(batch->GetMesh());
        if (batch->AllowInstancing())
        {
            if (graphics_->SetupProgram(pass, scene_, camera, nullptr, batch->GetMaterial(), light))
                graphics_->DrawInstancedActiveMesh(*batch, instanceBuffer_.get());
        }
        else
        {
            auto& nodes = batch->GetNodes();
            for (auto& node : nodes)
            {
                if (graphics_->SetupProgram(pass, scene_, camera, node, batch->GetMaterial(), light))
                    graphics_->DrawActiveMesh();
            }
        }
    }

    void Renderer::ShadowGenerationPass()
    {
        auto lights = scene_->GetLights();
        for (auto light : lights)
            if (light->DoShadows())
                light->GenerateShadowMaps(camera_);
    }

    void Renderer::OpaquePasses(std::vector<SceneNode*>& objs)
    {
        std::vector<Batch> batches;
        GenerateBatches(objs, batches);
        for (auto& batch : batches)
            Draw(&batch, &defaultOpaquePass_, nullptr, camera_);
        auto lights = scene_->GetLights();
        for (auto light : lights)
        {
            if (light->GetOnlyShadow())
                continue;
            std::vector<const Batch*> litBatches;
            GetLightedBatches(objs, batches, litBatches);
            for (auto batch : litBatches)
                Draw(batch, &litOpaquePass_, light, camera_);
        }
    }

    void Renderer::TransparentPasses(std::vector<SceneNode*>& objs)
    {
        std::vector<Batch> batches;
        GenerateBatches(objs, batches);
        for (auto& batch : batches)
            Draw(&batch, &defaultTransparentPass_, nullptr, camera_);
        auto lights = scene_->GetLights();
        for (auto light : lights)
        {
            if (light->GetOnlyShadow())
                continue;
            std::vector<const Batch*> litBatches;
            GetLightedBatches(objs, batches, litBatches);
            for (auto batch : litBatches)
                Draw(batch, &litTransparentPass_, light, camera_);
        }
    }

    void Renderer::FilterPass(std::vector<SceneNode*>& objs)
    {
        std::vector<Batch> batches;
        GenerateBatches(objs, batches);
        for (auto& batch : batches)
            Draw(&batch, &filterPass_, nullptr, camera_);
    }

    void Renderer::DebugPhysicsPass()
    {
        auto world = scene_->GetPhysicsWorld();
        if (world)
        {
            world->DrawDebug();
            auto debugRenderer = world->GetDebugRenderer();
            auto meshLines = debugRenderer->GetDebugLines();
            if (!meshLines->IsEmpty())
            {
                graphics_->SetMesh(meshLines.get());
                if (graphics_->SetupProgram(&debugPass_, scene_, camera_, nullptr, debugMaterial_.get(), nullptr))
                    graphics_->DrawActiveMesh();
                debugRenderer->Clear();
            }
        }
    }

    void Renderer::DebugRendererPass()
    {
        Renderer::SigDebugRenderer()->Run(debugRenderer_.get());
        auto meshLines = debugRenderer_->GetDebugLines();
        if (!meshLines->IsEmpty())
        {
            graphics_->SetMesh(meshLines.get());
            if (graphics_->SetupProgram(&debugPass_, scene_, camera_, nullptr, debugMaterial_.get(), nullptr))
                graphics_->DrawActiveMesh();
            debugRenderer_->Clear();
        }
    }

    void Renderer::Render(const Pass* pass, Mesh* mesh, Material* material)
    {
        graphics_->SetMesh(mesh);
        if (graphics_->SetupProgram(pass, nullptr, nullptr, nullptr, material, nullptr))
            graphics_->DrawActiveMesh();
    }

    void Renderer::Render(const Pass* pass, const Scene* scene, const Camera* camera, SceneNode* node, const Light* light)
    {
        graphics_->SetMesh(node->GetMesh().get());
        if (graphics_->SetupProgram(pass, scene, camera, node, node->GetMaterial().get(), light))
            graphics_->DrawActiveMesh();
    }

    void Renderer::Render(Window* window, Scene* scene)
    {
        if (scene)
            Render(window, scene, scene->GetMainCamera().get());
        else
            Render(window, nullptr, nullptr);
    }

    void Renderer::RenderOverlays()
    {
        auto overlays = scene_->GetOverlays();
        if (overlays && overlays->GetDrawablesNumber())
        {
            std::vector<SceneNode*> visibles;
            overlays->GetVisibleNodes(camera_, visibles);
            auto transparent = ExtractTransparent(visibles);
            RemoveFrom(visibles, transparent);
            if (!visibles.empty())
            {
                SortSolidFrontToBack(visibles);
                OpaquePasses(visibles);
                for (auto& obj : visibles)
                    obj->ClearUniform();
            }
            if (!transparent.empty())
            {
                SortTransparentBackToFront(transparent);
                TransparentPasses(transparent);
                for (auto& obj : transparent)
                    obj->ClearUniform();
            }
        }
    }

    void Renderer::RenderFiltered(const std::vector<SceneNode*>& objs)
    {
        auto filtered = objs;
        auto oldFrameBuffer = graphics_->GetFrameBuffer();
        auto filterFrameBuffer = window_->GetFilterFrameBuffer();
        do
        {
            graphics_->SetFrameBuffer(filterFrameBuffer);
            graphics_->SetClearColor(Color(0, 0, 0, 0));
            graphics_->ClearBuffers(true, false, false);
            auto filter = filtered[0]->GetFilter();
            auto it = std::partition(filtered.begin(), filtered.end(), [&](SceneNode * obj)
            {
                return obj->GetFilter().get() == filter.get();
            });

            std::vector<SceneNode*> nodesSameFilter(filtered.begin() , it);
            FilterPass(nodesSameFilter);
            filtered.erase(filtered.begin(), it);
            graphics_->SetFrameBuffer(oldFrameBuffer);
            filter->SetTexture(MaterialTexture::DIFFUSE_MAP, filterFrameBuffer->GetColorTexture());
            Renderer::GetPtr()->Render(&addPass_, QuadMesh::GetNDC().get(), filter.get());
        }
        while (filtered.size());
    }

    void Renderer::Render(Window* window, Scene* scene, Camera* camera)
    {
        CHECK_ASSERT(window && "window cannot be null");
        window_ = window;
        scene_ = scene;
        camera_ = camera;
        graphics_->SetWindow(window);
        if (!scene)
            graphics_->ClearAllBuffers();
        else if (scene->GetDrawablesNumber())
        {
            std::vector<SceneNode*> visibles;
            if (camera_)
            {
                scene->GetVisibleNodes(camera_, visibles);
                graphics_->SetClearColor(Color(1));
                ShadowGenerationPass();
            }
            else
                visibles = scene->GetDrawables();
            if (!visibles.empty())
            {
                auto filtered = ExtractFiltered(visibles);
                RemoveFrom(visibles, filtered);
                // we need to render to a framebuffer in order to blend the filters
                auto hasFiltered = !filtered.empty() && window_->UseFrameRender();
                auto transparent = ExtractTransparent(visibles);
                RemoveFrom(visibles, transparent);
                graphics_->SetClearColor(Color(scene->GetHorizonColor(), 1));
                graphics_->ClearAllBuffers();

                if (!visibles.empty())
                {
                    SortSolidFrontToBack(visibles);
                    OpaquePasses(visibles);
                    for (auto& obj : visibles)
                        obj->ClearUniform();
                }
                if (!transparent.empty())
                {
                    SortTransparentBackToFront(transparent);
                    TransparentPasses(transparent);
                    for (auto& obj : transparent)
                        obj->ClearUniform();
                }
                if (hasFiltered)
                {
                    RenderFiltered(filtered);
                    for (auto& obj : filtered)
                        obj->ClearUniform();
                }
                window->RenderFilters();
                if (debugPhysics_)
                    DebugPhysicsPass();
                DebugRendererPass();
                camera_ = overlaysCamera_.get();
                RenderOverlays();
            }
        }
        else
        {
            graphics_->ClearAllBuffers();
            camera_ = overlaysCamera_.get();
            RenderOverlays();
        }
        window->ShowMap();
    }

    SignalDebugRenderer::PSignal Renderer::SigDebugRenderer()
    {
        static SignalDebugRenderer::PSignal signalDebugRenderer(new SignalDebugRenderer);
        return signalDebugRenderer;
    }

    RendererContext Renderer::SetContext(RendererContext context)
    {
        auto old = context_;
        context_ = context;
        return old;
    }
}