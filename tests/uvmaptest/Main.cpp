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

#include "NSG.h"
int NSG_MAIN(int argc, char* argv[])
{
	using namespace NSG;

    // It is possible to use till 2 uv maps per mesh.
    // In Blender this corresponds to the first and second mapping defined in the mesh

	auto window = Window::Create("hiddenWindow", (int)WindowFlag::HIDDEN);
	{
		auto resource = Resource::GetOrCreate<ResourceFile>("data/aomap0.xml");
		AppData data(resource);
		CHECK_CONDITION(data.scenes_.size() == 1);
		auto scene = data.scenes_.at(0);
		auto obj = scene->GetChild<SceneNode>("Cube", true);
		CHECK_CONDITION(obj);
		auto mesh = obj->GetMesh();
		auto uv0Name = mesh->GetUVName(0);
		CHECK_CONDITION(uv0Name == "UVMap0");
		auto uv1Name = mesh->GetUVName(1);
		CHECK_CONDITION(uv1Name == "UVMap1");
		auto material = obj->GetMaterial();
		CHECK_CONDITION(material->IsReady());
		auto texture = material->GetTexture(MaterialTexture::AO_MAP);
		auto uvNameUsedByTexture = texture->GetUVName();
        // This texture uses first uv map defined in the mesh
		CHECK_CONDITION(uvNameUsedByTexture == uv0Name);
	}

	{
		auto resource = Resource::GetOrCreate<ResourceFile>("data/aomap1.xml");
		AppData data(resource);
		CHECK_CONDITION(data.scenes_.size() == 1);
		auto scene = data.scenes_.at(0);
		auto obj = scene->GetChild<SceneNode>("Cube", true);
		CHECK_CONDITION(obj);
		auto mesh = obj->GetMesh();
		auto uv0Name = mesh->GetUVName(0);
		CHECK_CONDITION(uv0Name == "UVMap0");
		auto uv1Name = mesh->GetUVName(1);
		CHECK_CONDITION(uv1Name == "UVMap1");
		auto material = obj->GetMaterial();
		CHECK_CONDITION(material->IsReady());
		auto texture = material->GetTexture(MaterialTexture::AO_MAP);
		auto uvNameUsedByTexture = texture->GetUVName();
        // This texture uses second uv map defined in the mesh
		CHECK_CONDITION(uvNameUsedByTexture == uv1Name);
	}

	{
		auto resource = Resource::GetOrCreate<ResourceFile>("data/uvmap275.xml");
		AppData data(resource);
		CHECK_CONDITION(data.scenes_.size() == 1);
		auto scene = data.scenes_.at(0);
		auto obj = scene->GetChild<SceneNode>("Cube", true);
		CHECK_CONDITION(obj);
		auto mesh = obj->GetMesh();
		auto uv0Name = mesh->GetUVName(0);
		CHECK_CONDITION(uv0Name == "UVMap1");
		auto uv1Name = mesh->GetUVName(1);
		CHECK_CONDITION(uv1Name == "UVMap2");
		auto material = obj->GetMaterial();
		CHECK_CONDITION(material->IsReady());
		auto texture = material->GetTexture(MaterialTexture::DIFFUSE_MAP);
		auto uvNameUsedByTexture = texture->GetUVName();
        // This texture uses second uv map defined in the mesh
		CHECK_CONDITION(uvNameUsedByTexture == uv1Name);
	}


}
