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
int NSG_MAIN(int argc, char* argv[])
{
    using namespace NSG;

    App app;

    auto window = app.GetOrCreateWindow("window", 100, 100, 150, 30);

    auto atlas = std::make_shared<FontAtlas>(window->GetWidth(), window->GetHeight());
    auto textCenter = atlas->GetOrCreateMesh("C Hello World!!!", CENTER_ALIGNMENT, MIDDLE_ALIGNMENT);
    auto textLeftTop = atlas->GetOrCreateMesh("LT Hello World!!!", LEFT_ALIGNMENT, TOP_ALIGNMENT);
    auto textRightTop = atlas->GetOrCreateMesh("RT Hello World!!!", RIGHT_ALIGNMENT, TOP_ALIGNMENT);
    auto textLeftBottom = atlas->GetOrCreateMesh("LB Hello World!!!", LEFT_ALIGNMENT, BOTTOM_ALIGNMENT);
    auto textRightBottom = atlas->GetOrCreateMesh("RB Hello World!!!", RIGHT_ALIGNMENT, BOTTOM_ALIGNMENT);

    auto scene = std::make_shared<Scene>("scene");
    auto nodeCenter = scene->GetOrCreateChild<SceneNode>("nodeCenter");
    auto nodeLeftTop = scene->GetOrCreateChild<SceneNode>("nodeLeftTop");
    auto nodeRightTop = scene->GetOrCreateChild<SceneNode>("nodeRightTop");
    auto nodeLeftBottom = scene->GetOrCreateChild<SceneNode>("nodeLeftBottom");
    auto nodeRightBottom = scene->GetOrCreateChild<SceneNode>("nodeRightBottom");

    auto material = app.GetOrCreateMaterial("material");
    material->SetColor(Color(1, 1, 1, 1));
    material->SetTexture0(atlas->GetTexture());
    auto technique = material->GetTechnique();
    auto pass = std::make_shared<Pass>();
    technique->Add(pass);
    pass->EnableDepthTest(false);
    pass->EnableStencilTest(false);
    auto program = std::make_shared<Program>(material);
    program->SetFlags((int)ProgramFlag::TEXT);
    pass->SetProgram(program);

    nodeCenter->SetMaterial(material);
    nodeLeftTop->SetMaterial(material);
    nodeRightTop->SetMaterial(material);
    nodeLeftBottom->SetMaterial(material);
    nodeRightBottom->SetMaterial(material);

    nodeCenter->SetMesh(textCenter);
    nodeLeftTop->SetMesh(textLeftTop);
    nodeRightTop->SetMesh(textRightTop);
    nodeLeftBottom->SetMesh(textLeftBottom);
    nodeRightBottom->SetMesh(textRightBottom);

    auto resizeSlot = window->signalViewChanged_->Connect([&](int width, int height)
    {
        atlas->SetViewSize(width, height);
        scene->SceneNode::MarkAsDirty();
    });

    auto renderSlot = window->signalRender_->Connect([&]()
    {
		scene->SceneNode::Render();
    });

	SceneNode* selectedNode = nullptr;
    HorizontalAlignment hAlign;
    VerticalAlignment vAlign;
    auto slotMouseDown = window->signalMouseDown_->Connect([&](int button, float x, float y)
    {
        Ray ray(Vector3(x, y, 0), VECTOR3_FORWARD);
        RayNodeResult closest;
        if (scene->GetClosestRayNodeIntersection(ray, closest))
        {
			selectedNode = closest.node_;
			auto mesh = std::dynamic_pointer_cast<TextMesh>(selectedNode->GetMesh());
			auto center = selectedNode->GetWorldBoundingBox().Center();
			auto size = selectedNode->GetWorldBoundingBox().Size();
			float y = center.y;
			if (y < 0)
				y -= size.y / 2.0f;
			else 
				y += size.y / 2.0f;
			selectedNode->SetPosition(Vector3(center.x, y, 0));
            mesh->GetAlignment(hAlign, vAlign);
			mesh->SetAlignment(CENTER_ALIGNMENT, MIDDLE_ALIGNMENT);
        }
    });

    auto slotMouseUp = window->signalMouseUp_->Connect([&](int button, float x, float y)
    {
		if (selectedNode)
		{
			selectedNode->SetPosition(Vertex3(0));
            selectedNode->SetOrientation(QUATERNION_IDENTITY);
            auto mesh = std::dynamic_pointer_cast<TextMesh>(selectedNode->GetMesh());
            mesh->SetAlignment(hAlign, vAlign);
			selectedNode = nullptr;
		}
    });

    SceneNode* colorNode = nullptr;
    auto slotMouseMoved = window->signalMouseMoved_->Connect([&](float x, float y)
    {
		if (colorNode)
			colorNode->SetMaterial(material);

        Ray ray(Vector3(x, y, 0), VECTOR3_FORWARD);
        RayNodeResult closest;
        if (scene->GetClosestRayNodeIntersection(ray, closest))
        {
            colorNode = closest.node_;
            auto material = colorNode->GetMaterial();
            auto clone = material->Clone("cloneMaterial");
            clone->SetColor(Color(1, 0, 0, 1));
            colorNode->SetMaterial(clone);
        }        
    });

    auto updateSlot = window->signalUpdate_->Connect([&](float deltaTime)
    {
    	if(selectedNode)
    	{
            const float ANGLE = glm::pi<float>() / 500.0f;
			const Quaternion ROTATION = glm::angleAxis(ANGLE, Vertex3(0, 0, 1));
			Quaternion q = selectedNode->GetOrientation();
			selectedNode->SetOrientation(q * ROTATION);
    	}
    });

    return app.Run();
}

