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
#include "SphereBehavior.h"

void SphereBehavior::Start()
{
	mesh_ = PSphereMesh(app_.CreateSphereMesh(3, 32));

	material_ = app_.CreateMaterial("sphere");
	PTexture texture(app_.GetOrCreateTextureFile("data/Earth.jpg"));
	material_->SetTexture0(texture);
}

void SphereBehavior::Update()
{
    float deltaTime = App::this_->GetDeltaTime();

	static float y_angle = 0;

    y_angle += glm::pi<float>()/10.0f * deltaTime;

	sceneNode_->SetOrientation(glm::angleAxis(y_angle, Vertex3(0, 0, 1)) * glm::angleAxis(y_angle, Vertex3(0, 1, 0)));
}


