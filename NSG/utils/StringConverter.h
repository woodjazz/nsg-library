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
#include "Types.h"
#include <string>

namespace NSG
{
	std::string ToString(const Vertex2& obj);
	Vertex2 ToVertex2(const std::string& buffer);

	std::string ToString(const Vertex3& obj);
	Vertex3 ToVertex3(const std::string& buffer);

	std::string ToString(const Vertex4& obj);
	Vertex4 ToVertex4(const std::string& buffer);

	std::string ToString(const Quaternion& obj);
	Quaternion ToQuaternion(const std::string& buffer);

	std::string ToString(const Matrix4& m);
	Matrix4 ToMatrix4(const std::string& buffer);

	std::string ToString(const BoundingBox& obj);
	BoundingBox ToBoundigBox(const std::string& buffer);

	std::string ToString(int obj);
	int ToInt(const char* value);
	int ToInt(const std::string& value);

	std::string ToString(size_t obj);

	const char* ToString(RenderPass obj);
	RenderPass ToRenderPass(const char* value);

	const char* ToString(CullFaceMode obj);
	CullFaceMode ToCullFaceMode(const char* obj);

	const char* ToString(TextureBlend obj);
	TextureBlend ToTextureBlend(const char* obj);

	const char* ToString(TextureType obj);
	TextureType ToTextureType(const char* obj);

	const char* ToString(BLEND_MODE obj);
	BLEND_MODE ToBlendMode(const char* obj);

	const char* ToString(FillMode obj);
	FillMode ToFillMode(const char* obj);

	const char* ToString(LightType obj);
	LightType ToLightType(const char* obj);

	const char* ToString(PhysicsShape obj);
	PhysicsShape ToPhysicsShape(const char* obj);

	const char* ToString(TextureWrapMode obj);
    TextureWrapMode ToTextureWrapMode(const char* obj);

    const char* ToString(TextureFilterMode obj);
    TextureFilterMode ToTextureFilterMode(const char* obj);

	std::string RemoveWhiteSpaces(const std::string& str);
}