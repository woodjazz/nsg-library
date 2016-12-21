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
#include "SharedFromPointer.h"
#include "Maths.h"

namespace NSG
{
    Renderer::Renderer()
        : context_(RenderingContext::Create()),
          scene_(nullptr),
          camera_(nullptr),
          showMapMaterial_(Material::Create("NSGShowMapMaterial")),
          debugPhysics_(false),
          debugMaterial_(Material::Create("NSGDebugMaterial")),
          debugRenderer_(std::make_shared<DebugRenderer>()),
          contextType_(RendererContext::DEFAULT),
          overlaysCamera_(std::make_shared<Camera>("NSGOverlays")),
          instanceBuffer_(new InstanceBuffer())
    {
        CHECK_CONDITION(instanceBuffer_->IsReady());
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

        showMapPass_.EnableDepthTest(false);
        showMapMaterial_->SetRenderPass(RenderPass::SHOW_TEXTURE0);
        showMapMaterial_->FlipYTextureCoords(true);

        debugMaterial_->SetRenderPass(RenderPass::VERTEXCOLOR);
        debugMaterial_->SetFillMode(FillMode::WIREFRAME);

        overlaysCamera_->EnableOrtho();
        overlaysCamera_->SetNearClip(-1000);
        overlaysCamera_->SetFarClip(1000);

        FrameBuffer::Flags frameBufferFlags((unsigned int)(FrameBuffer::COLOR | FrameBuffer::COLOR_USE_TEXTURE | FrameBuffer::DEPTH | FrameBuffer::Flag::DEPTH_USE_TEXTURE));
        filterFrameBuffer_ = PFrameBuffer(new FrameBuffer(GetUniqueName("RendererFilterFrameBuffer"), frameBufferFlags));
        frameBuffer_ = PFrameBuffer(new FrameBuffer(GetUniqueName("RendererFrameBuffer"), frameBufferFlags));
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


    void Renderer::GetLightedBatches(const std::vector<Batch>& iBatches, std::vector<const Batch*>& result) const
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
            auto da = a->GetGlobalPosition().Distance2(cameraPos);
            auto db = b->GetGlobalPosition().Distance2(cameraPos);
            return db < da;
        });
    }

    void Renderer::SortOverlaysBackToFront(std::vector<SceneNode*>& objs)
    {
        Vector3 cameraPos(overlaysCamera_->GetGlobalPosition());
        std::sort(objs.begin(), objs.end(), [&](const SceneNode * a, const SceneNode * b) -> bool
        {
            auto da = a->GetGlobalPosition().Distance2(cameraPos);
            auto db = b->GetGlobalPosition().Distance2(cameraPos);
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
            auto da = a->GetGlobalPosition().Distance2(cameraPos);
            auto db = b->GetGlobalPosition().Distance2(cameraPos);
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
                bool limitReached = batches.size() && batches.back().GetNodes().size() >= Batch::MaxNodesInBatch;
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
        context_->SetMesh(batch->GetMesh());
        if (batch->AllowInstancing())
        {
            if (context_->SetupProgram(pass, scene_, camera, nullptr, batch->GetMaterial(), light))
                context_->DrawInstancedActiveMesh(*batch, instanceBuffer_.get());
        }
        else
        {
            auto& nodes = batch->GetNodes();
            for (auto& node : nodes)
            {
                if (context_->SetupProgram(pass, scene_, camera, node, batch->GetMaterial(), light))
                    context_->DrawActiveMesh();
            }
        }
    }

    int Renderer::GetShadowFrameBufferSize(int split) const
    {
        CHECK_ASSERT(split < ShadowCamera::MAX_SPLITS);
        static const int SplitMapSize[ShadowCamera::MAX_SPLITS] = { 1024, 512, 256, 128 };
        return SplitMapSize[split];
    }

    int Renderer::CalculateSplits(const Camera* camera, float splits[ShadowCamera::MAX_SPLITS], const BoundingBox& camFrustumViewBox, const BoundingBox& receiversViewBox) const
    {
        auto camNear = camera->GetZNear();
        auto camFar  = camera->GetZFar();
        auto frustumDepth = camFar - camNear;
        #if 0
        splits[0] = camNear + frustumDepth * 0.25f;
        splits[1] = camNear + frustumDepth * 0.5f;
        splits[2] = camNear + frustumDepth * 0.75f;
        splits[3] = camFar;
        return 4;
        #else
        float shadowSplitLogFactor = camera->GetShadowSplitLogFactor();
        int nSplits = camera->GetMaxShadowSplits();

        if (camera->AutomaticSplits())
        {
            auto viewBox(receiversViewBox);
            viewBox.Transform(camera->GetView());
            viewBox.Clip(camFrustumViewBox);
            auto receiversDepth = viewBox.Size().z;
            auto frustumVisibilityFactor = receiversDepth / frustumDepth;
            frustumVisibilityFactor = Clamp(frustumVisibilityFactor, 0.f, 1.f);
            nSplits = (int)round(frustumVisibilityFactor * nSplits);
        }

        nSplits = Clamp(nSplits, 1, ShadowCamera::MAX_SPLITS);
        float zDistance = camFar - camNear;
        for (int i = 0; i < nSplits - 1; i++)
        {
            float factor = (i + 1.f) / (float)nSplits;
            splits[i] = Lerp(camNear + factor * zDistance,
                             camNear * powf(camFar / camNear, factor),
                             shadowSplitLogFactor);
        }
        splits[nSplits - 1] = camFar;
        return nSplits;
        #endif
    }

    void Renderer::GenerateShadowMapCubeFace(const Light* light)
    {
        auto shadowCamera = light->GetShadowCamera(0);
        std::vector<SceneNode*> shadowCasters;
        shadowCamera->GetVisiblesShadowCasters(shadowCasters);
        std::vector<Batch> batches;
        GenerateBatches(shadowCasters, batches);
        context_->ClearBuffers(true, true, false);
        for (auto& batch : batches)
            if (batch.GetMaterial()->CastShadow())
                DrawShadowPass(&batch, light, shadowCamera);
    }

    void Renderer::GenerateCubeShadowMap(const Camera* camera, const Light* light)
    {
        auto shadowFrameBuffer = light->GetShadowFrameBuffer(0);
        auto splitMapsize = GetShadowFrameBufferSize(0);
        shadowFrameBuffer->SetSize(splitMapsize, splitMapsize);
        if (shadowFrameBuffer->IsReady())
        {
            auto shadowCamera = light->GetShadowCamera(0);
            for (unsigned i = 0; i < (unsigned)CubeMapFace::MAX_CUBEMAP_FACES; i++)
            {
                TextureTarget face = (TextureTarget)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
                shadowCamera->SetCurrentCubeShadowMapFace(face);
                auto camFrustum = camera->GetFrustum();
                auto intersection = camFrustum->IsPointInside(light->GetGlobalPosition());
                bool genShadowMap = Intersection::OUTSIDE != intersection ||
                                    Intersection::OUTSIDE != camFrustum->IsInside(BoundingBox(*shadowCamera->GetFrustum()));
                if (genShadowMap)
                {
                    auto oldFrameBuffer = context_->SetFrameBuffer(shadowFrameBuffer, face);
                    GenerateShadowMapCubeFace(light);
                    context_->SetFrameBuffer(oldFrameBuffer);
                }
            }
        }
    }

    void Renderer::Generate2DShadowMap(int split, const Light* light)
    {
        auto shadowFrameBuffer = light->GetShadowFrameBuffer(split);
        auto splitMapsize = GetShadowFrameBufferSize(split);
        shadowFrameBuffer->SetSize(splitMapsize, splitMapsize);
        if (shadowFrameBuffer->IsReady())
        {
            auto shadowCamera = light->GetShadowCamera(split);
            std::vector<SceneNode*> shadowCasters;
            shadowCamera->GetVisiblesShadowCasters(shadowCasters);
            auto oldFrameBuffer = context_->SetFrameBuffer(shadowFrameBuffer);
            context_->ClearBuffers(true, true, false);
            if (!shadowCamera->IsDisabled())
            {
                std::vector<Batch> batches;
                GenerateBatches(shadowCasters, batches);
                for (auto& batch : batches)
                    if (batch.GetMaterial()->CastShadow())
                        DrawShadowPass(&batch, light, shadowCamera);
            }
            context_->SetFrameBuffer(oldFrameBuffer);
        }
    }

    void Renderer::GenerateShadowMaps(const Camera* camera, const Light* light)
    {
        switch (light->GetType())
        {
            case LightType::POINT:
                {
                    std::vector<SceneNode*> shadowCasters;
                    auto shadowCamera = light->GetShadowCamera(0);
                    shadowCamera->SetupPoint(camera);
                    GenerateCubeShadowMap(camera, light);
                    light->SetShadowSplits(1);
                }
                break;

            case LightType::SPOT:
                {
                    auto shadowCamera = light->GetShadowCamera(0);
                    shadowCamera->SetupSpot(camera);
                    auto intersection = camera->GetFrustum()->IsPointInside(light->GetGlobalPosition());
                    if (Intersection::OUTSIDE != intersection)
                        Generate2DShadowMap(0, light);
                    else if (Intersection::OUTSIDE != camera->GetFrustum()->IsInside(BoundingBox(*shadowCamera->GetFrustum())))
                        Generate2DShadowMap(0, light);
                    light->SetShadowSplits(1);
                }
                break;

            default: //DIRECTIONAL
                {
                    auto farZ = camera->GetZFar();
                    auto nearSplit = camera->GetZNear();
                    auto camFrustum = camera->GetFrustum();
                    BoundingBox camFullFrustumViewBox(*camFrustum);
                    auto receiversViewBox = Camera::GetViewBox(camFrustum.get(), light->GetScene().get(), true, false);
                    float splits[ShadowCamera::MAX_SPLITS];
                    int shadowSplits = CalculateSplits(camera, splits, camFullFrustumViewBox, receiversViewBox);
                    light->SetShadowSplits(shadowSplits);
                    int split = 0;
                    // Setup the shadow camera for each split
                    while (split < shadowSplits)
                    {
                        if (nearSplit > farZ)
                            break;
                        auto farSplit = std::min(farZ, splits[split]);
                        auto shadowCamera = light->GetShadowCamera(split);
                        shadowCamera->SetupDirectional(camera, nearSplit, farSplit);
                        nearSplit = farSplit;
                        ++split;
                    }

                    for (int i = 0; i < shadowSplits; i++)
                        Generate2DShadowMap(i, light);
                }
                break;
        }
    }

    void Renderer::ShadowGenerationPass()
    {
        auto lights = scene_->GetLights();
        for (auto light : lights)
            if (light->DoShadows())
                GenerateShadowMaps(camera_, light);
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
            GetLightedBatches(batches, litBatches);
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
            GetLightedBatches(batches, litBatches);
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
                context_->SetMesh(meshLines.get());
                if (context_->SetupProgram(&debugPass_, scene_, camera_, nullptr, debugMaterial_.get(), nullptr))
                    context_->DrawActiveMesh();
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
            context_->SetMesh(meshLines.get());
            if (context_->SetupProgram(&debugPass_, scene_, camera_, nullptr, debugMaterial_.get(), nullptr))
                context_->DrawActiveMesh();
            debugRenderer_->Clear();
        }
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
        if (filterFrameBuffer_->IsReady())
        {
            auto oldFrameBuffer = context_->GetFrameBuffer();
            auto filtered = objs;
            do
            {
                context_->SetFrameBuffer(filterFrameBuffer_.get());
                context_->SetClearColor(Color(0, 0, 0, 0));
                context_->ClearBuffers(true, false, false);
                auto filter = filtered[0]->GetFilter();
                auto it = std::partition(filtered.begin(), filtered.end(), [&](SceneNode * obj)
                {
                    return obj->GetFilter().get() == filter.get();
                });

                std::vector<SceneNode*> nodesSameFilter(filtered.begin() , it);
                FilterPass(nodesSameFilter);
                filtered.erase(filtered.begin(), it);
                context_->SetFrameBuffer(oldFrameBuffer);
                filter->SetTexture(MaterialTexture::DIFFUSE_MAP, filterFrameBuffer_->GetColorTexture());
                Render(&addPass_, QuadMesh::GetNDC().get(), filter.get());
            }
            while (filtered.size());
        }
    }

    void Renderer::ApplyPostProcessing()
    {
        if (camera_)
        {
            if (filterFrameBuffer_->IsReady())
            {
                auto filters = camera_->GetFilters();
                for (auto filter : filters)
                {
                    context_->SetFrameBuffer(filterFrameBuffer_.get());
                    filter->SetTexture(MaterialTexture::DIFFUSE_MAP, frameBuffer_->GetColorTexture());
                    filter->FlipYTextureCoords(true);
                    Render(&showMapPass_, QuadMesh::GetNDC().get(), filter);
                    context_->SetFrameBuffer(frameBuffer_.get());
                    showMapMaterial_->SetTexture(filterFrameBuffer_->GetColorTexture());
                    Render(&showMapPass_, QuadMesh::GetNDC().get(), showMapMaterial_.get());
                }
            }
        }
    }

    void Renderer::Render(const Pass* pass, Mesh* mesh, Material* material)
    {
        context_->SetMesh(mesh);
        if (context_->SetupProgram(pass, nullptr, nullptr, nullptr, material, nullptr))
            context_->DrawActiveMesh();
    }

    void Renderer::Render(const Pass* pass, const Scene* scene, const Camera* camera, SceneNode* node, const Light* light)
    {
        context_->SetMesh(node->GetMesh().get());
        if (context_->SetupProgram(pass, scene, camera, node, node->GetMaterial().get(), light))
            context_->DrawActiveMesh();
    }

    void Renderer::Render(Window* window, Scene* scene)
    {
        if (scene)
            Render(window, scene, scene->GetMainCamera().get());
        else
            Render(window, nullptr, nullptr);
    }

    static Window* nullWindow = nullptr;
    void Renderer::Render(FrameBuffer* frameBuffer, Scene* scene, Camera* camera)
    {
        auto oldFrameBuffer = context_->SetFrameBuffer(frameBuffer);
        Render(nullWindow, scene, camera);
        context_->SetFrameBuffer(oldFrameBuffer);
    }

    void Renderer::Render(FrameBuffer* frameBuffer, Scene* scene)
    {
        auto oldFrameBuffer = context_->SetFrameBuffer(frameBuffer);
        if (scene)
            Render(nullWindow, scene, scene->GetMainCamera().get());
        else
            Render(nullWindow, nullptr, nullptr);
        context_->SetFrameBuffer(oldFrameBuffer);
    }

    void Renderer::Render(Window* window, Scene* scene, Camera* camera)
    {
        bool useFrameBuffer = false;
        int width = 0;
        int height = 0;
        if (window)
        {
            window->SetContext();
            context_->SetFrameBuffer(nullptr);
            context_->SetViewport(*window);
            width = window->GetWidth();
            height = window->GetHeight();
        }
        else
        {
            auto currentFrameBuffer = context_->GetFrameBuffer();
            CHECK_CONDITION(currentFrameBuffer);
            width = currentFrameBuffer->GetWidth();
            height = currentFrameBuffer->GetHeight();
        }
        scene_ = scene;
        camera_ = camera;
        if (!scene)
            context_->ClearAllBuffers();
        else if (scene->GetDrawablesNumber())
        {
            std::vector<SceneNode*> visibles;
            if (camera_)
            {
                scene->GetVisibleNodes(camera_, visibles);
                context_->SetClearColor(Color(1));
                ShadowGenerationPass();
            }
            else
                visibles = scene->GetDrawables();
            if (!visibles.empty())
            {
                bool hasPostProcessing = camera && camera->HasPostProcessing();
                auto filtered = ExtractFiltered(visibles);
                if (hasPostProcessing || !filtered.empty())
                {
                    frameBuffer_->SetSize(width, height);
                    filterFrameBuffer_->SetSize(width, height);
                    if (frameBuffer_->IsReady())
                    {
                        context_->SetFrameBuffer(frameBuffer_.get());
                        useFrameBuffer = true;
                    }
                }
                RemoveFrom(visibles, filtered);
                auto transparent = ExtractTransparent(visibles);
                RemoveFrom(visibles, transparent);
                context_->SetClearColor(scene->GetHorizonColor());
                context_->ClearAllBuffers();
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
                if (!filtered.empty())
                {
                    RenderFiltered(filtered);
                    for (auto& obj : filtered)
                        obj->ClearUniform();
                }
                ApplyPostProcessing();
                if (debugPhysics_)
                    DebugPhysicsPass();
                DebugRendererPass();
                camera_ = overlaysCamera_.get();
                RenderOverlays();
            }
        }
        else
        {
            context_->ClearAllBuffers();
            camera_ = overlaysCamera_.get();
            RenderOverlays();
        }

        if (useFrameBuffer)
        {
            context_->SetFrameBuffer(nullptr); //use system framebuffer to show the texture
            showMapMaterial_->SetTexture(frameBuffer_->GetColorTexture());
            Render(&showMapPass_, QuadMesh::GetNDC().get(), showMapMaterial_.get());
        }
    }

    SignalDebugRenderer::PSignal Renderer::SigDebugRenderer()
    {
        static SignalDebugRenderer::PSignal signalDebugRenderer(new SignalDebugRenderer);
        return signalDebugRenderer;
    }

    RendererContext Renderer::SetContextType(RendererContext type)
    {
        std::swap(contextType_, type);
        return type;
    }
}
