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

#include "NSG.h"
#include <deque>
int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;

    App app;

    std::deque<Vertex3> boxControlPoints;

    auto window = app.GetOrCreateWindow("window", 100, 100, 50, 30);
    auto scene = std::make_shared<Scene>("scene000");
    auto camera = scene->GetOrCreateChild<Camera>(GetUniqueName());
    camera->SetAspectRatio(window->GetWidth(), window->GetHeight());
    auto resizeSlot = window->signalViewChanged_->Connect([&](int width, int height)
    {
        camera->SetAspectRatio(width, height);
    });
    camera->SetPosition(Vertex3(0, 0, 10));

    auto box = scene->GetOrCreateChild<SceneNode>(GetUniqueName());
    box->SetScale(Vertex3(3, 3, 3));
    boxControlPoints.push_back(Vertex3(-5.0f, 0.0f, 0.0f));
    boxControlPoints.push_back(Vertex3(0.0f, 0.0f, 5.0f));
    boxControlPoints.push_back(Vertex3(5.0f, 0.0f, 0.0f));
    boxControlPoints.push_back(Vertex3(0.0f, 0.0f, -5.0f));

    auto boxMesh = app.CreateBoxMesh(1, 1, 1, 2, 2, 2);
    auto boxMaterial = app.GetOrCreateMaterial(GetUniqueName());
    auto cubeResource = std::make_shared<ResourceFile>("data/cube.png");
    auto cubeTexture = std::make_shared<Texture>(cubeResource);
    cubeTexture->SetFlags((int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
    boxMaterial->SetTexture0(cubeTexture);

    auto sphere = scene->GetOrCreateChild<SceneNode>(GetUniqueName());
    auto sphereMesh = app.CreateSphereMesh(3, 32);
    auto sphereMaterial = app.GetOrCreateMaterial(GetUniqueName());
    auto sphereResource = std::make_shared<ResourceFile>("data/Earth.jpg");
    auto texture(std::make_shared<Texture>(sphereResource));
    texture->SetFlags((int)TextureFlag::GENERATE_MIPMAPS | (int)TextureFlag::INVERT_Y);
    sphereMaterial->SetTexture0(texture);

    auto showTexture = std::make_shared<ShowTexture>();

    auto technique = std::make_shared<Technique>();

    PPass normalPass(new Pass);
    auto normalProgram(app.GetOrCreateProgram(GetUniqueName()));
    normalProgram->SetFlags((int)ProgramFlag::UNLIT | (int)ProgramFlag::DIFFUSEMAP);
    normalPass->SetProgram(normalProgram);

    PPass depthPass(new Pass);
    depthPass->EnableColorBuffer(false);
    auto depthProgram(app.GetOrCreateProgram(GetUniqueName()));
    depthProgram->SetFlags((int)ProgramFlag::STENCIL);
    depthPass->SetProgram(depthProgram);

    PTexture tx000;
    PFilter boxFilter;

    {
        //box passes
        PPass2Texture pass2Texture(new Pass2Texture(1024, 1024));
        technique->Add(pass2Texture);

        tx000 = pass2Texture->GetTexture();
        boxFilter = PFilter(new Filter(GetUniqueName(), pass2Texture->GetTexture(), 1024, 1024));

        static const char*
        fShader = STRINGIFY(void main()
        {
            vec2 texcoord = v_texcoord0;
            texcoord.x += sin(texcoord.y * 4.0 * 2.0 * 3.14159 + u_material.shininess) / 100.0;
            gl_FragColor = texture2D(u_texture0, texcoord);
        });

        PResourceMemory resource(new ResourceMemory(fShader, strlen(fShader)));
        boxFilter->GetProgram()->SetFragmentShader(resource);

        pass2Texture->Add(depthPass, sphere.get(), nullptr, sphereMesh);
        pass2Texture->Add(normalPass, box.get(), boxMaterial, boxMesh);

        PPassFilter filterPass(new PassFilter(boxFilter));
        technique->Add(filterPass);
    }

    #if 1
    PFilter sphereBlendFilter;
    {
        //sphere passes

        PPass2Texture pass2Texture(new Pass2Texture(1024, 1024));
        technique->Add(pass2Texture);
        pass2Texture->Add(depthPass, box.get(), nullptr, boxMesh);
        pass2Texture->Add(normalPass, sphere.get(), sphereMaterial, sphereMesh);

        PFilter blurFilter(new FilterBlur(pass2Texture->GetTexture(), 16, 16));

        PPassFilter filterPass(new PassFilter(blurFilter));
        technique->Add(filterPass);


        sphereBlendFilter = PFilter(new FilterBlend(blurFilter->GetTexture(), pass2Texture->GetTexture(), 1024, 1024));

        PPassFilter passBlend(new PassFilter(sphereBlendFilter));
        technique->Add(passBlend);

        //showTexture_->SetNormal(pass2Texture->GetTexture());
    }

    PFilter blendFilter(new FilterBlend(sphereBlendFilter->GetTexture(), boxFilter->GetTexture(), 1024, 1024));
    PPassFilter passBlend(new PassFilter(blendFilter));
    technique->Add(passBlend);
    #endif

    showTexture->SetNormal(blendFilter->GetTexture());
    //showTexture->SetNormal(app.GetWhiteTexture());
    //showTexture->SetNormal(sphereBlendFilter->GetTexture());
    //showTexture->SetNormal(tx000);
    //showTexture->SetNormal(boxFilter->GetTexture());

    auto slotUpdate = window->signalUpdate_->Connect([&](float deltaTime)
    {
        {
            static float delta1 = 0;

            Vertex3 position = glm::catmullRom(
                                   boxControlPoints[0],
                                   boxControlPoints[1],
                                   boxControlPoints[2],
                                   boxControlPoints[3],
                                   delta1);

            box->SetPosition(position);

            delta1 += deltaTime * 0.3f;

            if (delta1 > 1)
            {
                delta1 = 0;
                Vertex3 p = boxControlPoints.front();
                boxControlPoints.pop_front();
                boxControlPoints.push_back(p);
            }

            static float move = -1;
            move += deltaTime * TWO_PI * 0.25f;  // 1/4 of a wave cycle per second
            boxMaterial->SetShininess(move);
        }

        {
            static float y_angle = 0;
            y_angle += glm::pi<float>() / 10.0f * deltaTime;
            sphere->SetOrientation(glm::angleAxis(y_angle, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle, Vertex3(0, 1, 0)));
        }
    });

    auto renderSlot = window->signalRender_->Connect([&]()
    {
        technique->Render(camera.get());
        showTexture->Show();
    });

    return app.Run();
}
