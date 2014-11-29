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
using namespace NSG;

struct Sample : App
{
    PScene scene_;
    PLight light_;
    PMesh sphereMesh_;
    PMesh cubeMesh_;
    PTexture diffuseMap_;
    PTexture normalMap_;
    PTexture lightMap_;
    PMaterial material_;
    PProgram program_;
	PPass pass_;
    PSceneNode node_;
	PCameraControl control_;
    SignalStart::PSlot slotStart_;

    Sample()
    {
        AppConfiguration::this_->showStatistics_ = false;

		slotStart_ = signalStart_->Connect([&](int argc, char* argv[])
		{
			scene_ = GetOrCreateScene("scene000");
			SetCurrentScene(scene_);
			scene_->SetAmbientColor(Color(0));

			PCamera camera = scene_->GetOrCreateChild<Camera>("camera");
			camera->SetPosition(Vertex3(0, 0, 10));
			control_ = PCameraControl(new CameraControl(camera));
			camera->Activate();

			light_ = camera->GetOrCreateChild<Light>("pointlight");

			sphereMesh_ = CreateSphereMesh();
			cubeMesh_ = CreateBoxMesh();

			diffuseMap_ = GetOrCreateTextureFile("data/wall.jpg");
			normalMap_ = GetOrCreateTextureFile("data/wallnormalmap.jpg");
			lightMap_ = GetOrCreateTextureFile("data/lightmap.png");
			material_ = GetOrCreateMaterial("material");
			material_->SetDiffuseMap(diffuseMap_);
			material_->SetNormalMap(normalMap_);
			material_->SetLightMap(lightMap_);
			program_ = GetOrCreateProgram("program0");
			PTechnique technique(new Technique);
			pass_ = PPass(new Pass);
			technique->Add(pass_);
			pass_->SetProgram(program_);
			material_->SetTechnique(technique);

			node_ = scene_->GetOrCreateChild<SceneNode>("node");
			node_->SetPosition(Vertex3(0, 0, 0));
			node_->Set(material_);
			node_->Set(sphereMesh_);
		});
    }


    void RenderGUIWindow() override
    {
        using namespace IMGUI;

        IMGUIBeginHorizontal();
        {
            IMGUIBeginVertical(25, 100);
            {
                const float Y_PERCENTAGE = 10;

                ProgramFlags flags = program_->GetFlags();

                {
                    bool flag = flags & (int)ProgramFlag::PER_VERTEX_LIGHTING ? true : false;
                    if (IMGUICheckButton(flag, flag ? "Disable PVL" : "Enable PVL", 100, Y_PERCENTAGE))
                        flags |= (int)ProgramFlag::PER_VERTEX_LIGHTING;
                    else
                        flags &= ~(int)ProgramFlag::PER_VERTEX_LIGHTING;
                }

                {
                    bool flag = flags & (int)ProgramFlag::PER_PIXEL_LIGHTING ? true : false;
                    if (IMGUICheckButton(flag, flag ? "Disable PPL" : "Enable PPL", 100, Y_PERCENTAGE))
                        flags |= (int)ProgramFlag::PER_PIXEL_LIGHTING;
                    else
                        flags &= ~(int)ProgramFlag::PER_PIXEL_LIGHTING;
                }

                {
                    bool flag = flags & (int)ProgramFlag::BLEND ? true : false;
                    if (IMGUICheckButton(flag, flag ? "Disable Blend" : "Enable Blend", 100, Y_PERCENTAGE))
                        flags |= (int)ProgramFlag::BLEND;
                    else
                        flags &= ~(int)ProgramFlag::BLEND;
                }

                {
                    bool flag = flags & (int)ProgramFlag::BLUR ? true : false;
                    if (IMGUICheckButton(flag, flag ? "Disable Blur" : "Enable Blur", 100, Y_PERCENTAGE))
                        flags |= (int)ProgramFlag::BLUR;
                    else
                        flags &= ~(int)ProgramFlag::BLUR;
                }

                {
                    bool flag = flags & (int)ProgramFlag::TEXT ? true : false;
                    if (IMGUICheckButton(flag, flag ? "Disable TEXT" : "Enable TEXT", 100, Y_PERCENTAGE))
                        flags |= (int)ProgramFlag::TEXT;
                    else
                        flags &= ~(int)ProgramFlag::TEXT;
                }

                {
                    bool flag = flags & (int)ProgramFlag::SHOW_TEXTURE0 ? true : false;
                    if (IMGUICheckButton(flag, flag ? "Disable SHOW_TEXTURE0" : "Enable SHOW_TEXTURE0", 100, Y_PERCENTAGE))
                        flags |= (int)ProgramFlag::SHOW_TEXTURE0;
                    else
                        flags &= ~(int)ProgramFlag::SHOW_TEXTURE0;
                }

                {
                    bool flag = flags & (int)ProgramFlag::STENCIL ? true : false;
                    if (IMGUICheckButton(flag, flag ? "Disable STENCIL" : "Enable STENCIL", 100, Y_PERCENTAGE))
                        flags |= (int)ProgramFlag::STENCIL;
                    else
                        flags &= ~(int)ProgramFlag::STENCIL;
                }

                {
                    bool flag = flags & (int)ProgramFlag::NORMALMAP ? true : false;
                    if (IMGUICheckButton(flag, flag ? "Disable NORMALMAP" : "Enable NORMALMAP", 100, Y_PERCENTAGE))
                        flags |= (int)ProgramFlag::NORMALMAP;
                    else
                        flags &= ~(int)ProgramFlag::NORMALMAP;
                }

                {
                    bool flag = flags & (int)ProgramFlag::LIGHTMAP ? true : false;
                    if (IMGUICheckButton(flag, flag ? "Disable LIGHTMAP" : "Enable LIGHTMAP", 100, Y_PERCENTAGE))
                        flags |= (int)ProgramFlag::LIGHTMAP;
                    else
                        flags &= ~(int)ProgramFlag::LIGHTMAP;
                }

                {
                    bool flag = flags & (int)ProgramFlag::UNLIT ? true : false;
                    if (IMGUICheckButton(flag, flag ? "Disable UNLIT" : "Enable UNLIT", 100, Y_PERCENTAGE))
                        flags |= (int)ProgramFlag::UNLIT;
                    else
                        flags &= ~(int)ProgramFlag::UNLIT;
                }

                program_->SetFlags(flags);
            }
            IMGUIEndArea();

            IMGUISpacer(50, 100);

            IMGUIBeginVertical(25, 100);
            {
				{
					bool isSphere = node_->GetMesh() == sphereMesh_;
					isSphere = IMGUICheckButton(isSphere, isSphere ? "Cube" : "Sphere", 100, 25);
					if (isSphere)
						node_->Set(sphereMesh_);
					else
						node_->Set(cubeMesh_);
				}

				{
					DrawMode drawMode = pass_->GetDrawMode();
					bool solid = drawMode == DrawMode::SOLID;
					solid = IMGUICheckButton(solid, solid ? "Wireframe" : "Solid", 100, 25);
					if (solid)
						pass_->SetDrawMode(DrawMode::SOLID);
					else
						pass_->SetDrawMode(DrawMode::WIREFRAME);
				}

            }
            IMGUIEndArea();
        }
        IMGUIEndArea();
    }

};

NSG_MAIN(Sample);
