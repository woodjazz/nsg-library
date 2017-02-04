/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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

#include "NSG.h"
using namespace NSG;
PScene scene;

PSceneNode CreateObject(PMesh mesh, Color color, const Vector3& pos)
{
    auto obj = scene->CreateChild<SceneNode>();
    obj->SetGlobalPosition(pos);
    auto material = Material::GetOrCreate(mesh->GetName());
    material->SetDiffuseColor(color);
    //material->SetBias(.9f);
    //material->SetSpecularColor(Color(0));
    material->SetRenderPass(RenderPass::LIT);
    //material->SetRenderPass(RenderPass::UNLIT);
    obj->SetMesh(mesh);
    obj->SetMaterial(material);
    return obj;
}

static void SetFog(PScene scene)
{
    scene->SetHorizonColor(Color(0, 1, 0));
    scene->EnableFog(true);
    scene->SetFogDepth(150);
    scene->SetFogStart(15);
    //scene->SetFogHeight(0.1f);
}

#define TEST1
int NSG_MAIN(int argc, char* argv[])
{
    //return 0;

    using namespace NSG;
    auto window = Window::Create();
    scene = std::make_shared<Scene>("scene");

#if 0
    Editor editor;
    editor.SetWindow(window);
    editor.SetScene(scene);
#endif
    //SetFog(scene);
    auto light = scene->CreateChild<Light>("Light");
    ShadowMapDebug shadowMapDebug(window, light);
    light->SetType(LightType::DIRECTIONAL);
    light->SetShadowColor(Color::Black);
    //light->SetBias(0.1000f);
    auto camera = scene->CreateChild<Camera>();
    camera->SetWindow(window);
    camera->SetFarClip(100);
    #ifdef TEST1
    light->SetGlobalLookAtPosition(Vector3(-10, -1, 0));
    camera->SetPosition(Vector3(6.552f, 13.163f, -3.545f));
    //camera->SetLocalLookAtPosition(Vector3(0, 0, -10));
    camera->SetOrientation(Quaternion(0.849f, -0.236f, 0.454f, 0.126f));
    //camera->SetShadowSplitLogFactor(1.f);
    #else
    light->SetGlobalLookAtPosition(Vector3(0, -5, -10));
    camera->SetPosition(Vector3(0, 20, 20));
    camera->SetLocalLookAtPosition(Vector3(0, -1, -1));
    #endif
    auto control = std::make_shared<CameraControl>(camera);
    auto plane = Mesh::Create<BoxMesh>();
    plane->Set(60, 0.1f, 500);
    auto floor = CreateObject(plane, Color(0.058f, 0.58f, 0.98f), Vector3(0));
    floor->GetMaterial()->CastShadow(false);
    //floor->GetMaterial()->SetAlphaForSpecular(0);
    #ifdef TEST1
    const int MAX_OBJS = 50;
    for (int i = 0; i < MAX_OBJS / 2; i++)
    {
        float z = -i * 2.5f;
        CreateObject(Mesh::GetOrCreate<SphereMesh>("Sphere"), Color::Red, Vector3(-1, 1, z));
        //CreateObject(Mesh::GetOrCreate<BoxMesh>("Box"), COLOR_BLUE, Vector3(1, 1, z));
    }

    for (int i = MAX_OBJS / 2; i < MAX_OBJS; i++)
    {
        float z = -i * 2.5f;
        //auto sphere = CreateObject(Mesh::GetOrCreate<SphereMesh>("Sphere"), COLOR_RED, Vector3(-1, 1, z));
        //sphere->GetMaterial()->SetSpecularColor(COLOR_WHITE);
        CreateObject(Mesh::GetOrCreate<BoxMesh>("Box"), Color::Blue, Vector3(1, 1, z));
    }
    #else
    //camera->SetShadowSplits(1);
    #if 1
    auto mesh = Mesh::GetOrCreate<SphereMesh>("Sphere");
    mesh->Set(10, 20);
    auto sphere = CreateObject(mesh, COLOR_RED, Vector3(0, 10, 0));
    sphere->GetMaterial()->SetSpecularColor(COLOR_WHITE);
    #endif
    //CreateObject(Mesh::Create<BoxMesh>(), COLOR_BLUE, Vector3(0, 1, 0));
    #endif

    auto debugSlot = Renderer::GetPtr()->SigDebugRenderer()->Connect([&](DebugRenderer * renderer)
    {
        #if 0
        BoundingBox bb(-1, 1);

        camera1.SetNearClip(5);
        camera1.SetFarClip(50);
        camera1.Debug(renderer, Color(1, 0.6f, 0.6f, 1));
        auto f0 = camera1.GetFrustumSplit(5, 10);
        auto f1 = camera1.GetFrustumSplit(11, 15);
        auto f2 = camera1.GetFrustumSplit(16, 20);
        auto f3 = camera1.GetFrustumSplit(21, 50);
        #if 0
        BoundingBox b0(*f0);
        BoundingBox b1(*f1);
        BoundingBox b2(*f2);
        BoundingBox b3(*f3);

        b0.Debug(renderer, Color(1, 0, 0, 1));
        b1.Debug(renderer, Color(0, 1, 0, 1));
        b2.Debug(renderer, Color(0, 0, 1, 1));
        b3.Debug(renderer, Color(1.0f, 69.0f / 255.0f, 0.0f, 1.f));
        #else
        f0->Debug(VECTOR3_ZERO, renderer, Color(1, 0, 0, 1));
        f1->Debug(VECTOR3_ZERO, renderer, Color(0, 1, 0, 1));
        f2->Debug(VECTOR3_ZERO, renderer, Color(0, 0, 1, 1));
        f3->Debug(VECTOR3_ZERO, renderer, Color(1.0f, 69.0f / 255.0f, 0.0f, 1.f));
        #endif
        #endif
        //bb.Debug(renderer, Color(1, 0, 0, 1));
    });

    auto engine = Engine::Create();
    auto slotUpdate = engine->SigUpdate()->Connect([&](float deltaTime)
    {
        //LOGI("%s", ToString(camera->GetView() * Vector4(0, 0, -50, 1)).c_str());
        //LOGI("%s", ToString(camera->GetPosition()).c_str());
        //LOGI("q=%s", ToString(camera->GetOrientation()).c_str());
    });
    window->SetScene(scene);
    return engine->Run();
}

