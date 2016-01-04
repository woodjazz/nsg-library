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
#include "Sun.h"
#include "Planet.h"

Sun::Sun(PScene scene)
: sun_(scene->CreateChild<Light>())
{
    sun_->EnableShadows(false);
    //auto material(Material::Create());
    //material->SetRenderPass(RenderPass::LIT);
    //material->SetDiffuseColor(ColorRGB(1, 0, 0));
    //sun_->SetMaterial(material);
    //sun_->SetMesh(Planet::GetMesh());
    //sun_->SetPosition(Vertex3(10, 10, 10));
    sun_->SetType(LightType::DIRECTIONAL);
    sun_->SetGlobalLookAtDirection(Vector3(-0.3f, 0.4f, 1));
}

Sun::~Sun()
{

}
