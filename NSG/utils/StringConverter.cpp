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
#include "StringConverter.h"
#include "BoundingBox.h"
#include "Util.h"
#include "Check.h"
#ifndef IS_TARGET_WINDOWS
#include <unistd.h>
#include <cerrno>
#else
#include <windows.h>
#define snprintf _snprintf
#endif

#include <array>
#include <cctype>
#include <cstring>

namespace NSG
{
	template<typename T>
	const char* ToString(T obj, const char* mapping[])
	{
		auto idx = (int)obj;
		CHECK_ASSERT(idx >= 0 && idx < (int)T::MAX_INDEX);
		return mapping[idx];
	}

    template<typename T>
    T ToType(const char* value, const char* mapping[])
    {
		for (int i = 0; i<(int)T::MAX_INDEX; i++)
            if(strcmp(mapping[i], value) == 0)
                return (T)i;
        CHECK_ASSERT(!"Failed to translate string!!!");
        return (T)0;
    }

    std::string ToString(const Vertex2& obj)
    {
        using namespace std;
        const int MaxBuffer = 100;
        char buffer[MaxBuffer];
        snprintf(buffer, MaxBuffer, "[%g,%g]", obj.x, obj.y);
        return buffer;
    }

    Vertex2 ToVertex2(const std::string& buffer)
    {
        Vertex2 obj;
        sscanf(buffer.c_str(), "[%g,%g]", &obj.x, &obj.y);
        return obj;
    }

    std::string ToString(const Vertex3& obj)
    {
        using namespace std;
        const int MaxBuffer = 100;
        char buffer[MaxBuffer];
        snprintf(buffer, MaxBuffer, "[%g,%g,%g]", obj.x, obj.y, obj.z);
        return buffer;
    }

    Vertex3 ToVertex3(const std::string& buffer)
    {
        Vertex3 obj;
        sscanf(buffer.c_str(), "[%g,%g,%g]", &obj.x, &obj.y, &obj.z);
        return obj;
    }

    std::string ToString(const Vertex4& obj)
    {
        using namespace std;
        const int MaxBuffer = 100;
        char buffer[MaxBuffer];
        snprintf(buffer, MaxBuffer, "[%g,%g,%g,%g]", obj.x, obj.y, obj.z, obj.w);
        return buffer;
    }

    std::string ToString(const BoundingBox& obj)
    {
        using namespace std;
        const int MaxBuffer = 100;
        char buffer[MaxBuffer];
        snprintf(buffer, MaxBuffer, "[%g,%g,%g][%g,%g,%g]", obj.min_.x, obj.min_.y, obj.min_.z, obj.max_.x, obj.max_.y, obj.max_.z);
        return buffer;
    }

    BoundingBox ToBoundigBox(const std::string& buffer)
    {
		Vector3 min;
		Vector3 max;
        sscanf(buffer.c_str(), "[%g,%g,%g][%g,%g,%g]", &min.x, &min.y, &min.z, &max.x, &max.y, &max.z);
		return BoundingBox(min, max);
    }

    Vertex4 ToVertex4(const std::string& buffer)
    {
        Vertex4 obj;
        sscanf(buffer.c_str(), "[%g,%g,%g,%g]", &obj.x, &obj.y, &obj.z, &obj.w);
        return obj;
    }

    std::string ToString(const Quaternion& obj)
    {
        using namespace std;
        const int MaxBuffer = 100;
        char buffer[MaxBuffer];
        snprintf(buffer, MaxBuffer, "[%g,%g,%g,%g]", obj.w, obj.x, obj.y, obj.z);
        return buffer;
    }

    Quaternion ToQuaternion(const std::string& buffer)
    {
        Quaternion obj;
        sscanf(buffer.c_str(), "[%g,%g,%g,%g]", &obj.w, &obj.x, &obj.y, &obj.z);
        return obj;
    }

    std::string ToString(const Matrix4& m)
    {
        using namespace std;
        const int MaxBuffer = 400;
        char buffer[MaxBuffer];
        snprintf(buffer, MaxBuffer, "[ %s %s %s %s ]", ToString(m.Column(0)).c_str(), ToString(m.Column(1)).c_str(), ToString(m.Column(2)).c_str(), ToString(m.Column(3)).c_str());
        return buffer;
    }

    Matrix4 ToMatrix4(const std::string& buffer)
    {
        const int MaxBuffer = 100;
        char buffer0[MaxBuffer];
        char buffer1[MaxBuffer];
        char buffer2[MaxBuffer];
        char buffer3[MaxBuffer];
        sscanf(buffer.c_str(), "[ %s %s %s %s ]", buffer0, buffer1, buffer2, buffer3);
        return Matrix4(ToVertex4(buffer0), ToVertex4(buffer1), ToVertex4(buffer2), ToVertex4(buffer3));
    }

    std::string ToString(int obj)
    {
        using namespace std;
        const int MaxBuffer = 50;
        char buffer[MaxBuffer];
        snprintf(buffer, MaxBuffer, "%d", obj);
        return buffer;
    }

    int ToInt(const std::string& value)
    {
        return ToInt(value.c_str());
    }

    int ToInt(const char* value)
    {
        if (!value)
            return 0;
        return strtol(value, 0, 10);
    }

    std::string ToString(size_t obj)
    {
        using namespace std;
        const int MaxBuffer = 50;
        char buffer[MaxBuffer];
		snprintf(buffer, MaxBuffer, "%u", (unsigned)obj);
        return buffer;
    }

	std::string ToString(float obj)
	{
		using namespace std;
		const int MaxBuffer = 50;
		char buffer[MaxBuffer];
		snprintf(buffer, MaxBuffer, "%f", obj);
		return buffer;
	}

    static const char* RenderPassStr[] =
    {
        "VERTEXCOLOR",
        "UNLIT",
        "LIT", 
        "TEXT", 
        "BLEND", 
        "BLUR", 
        "WAVE", 
        "SHOW_TEXTURE0"
    };

    const char* ToString(RenderPass obj)
    {
        return ToString<RenderPass>(obj, RenderPassStr);
    }

    RenderPass ToRenderPass(const char* value)
    {
        return ToType<RenderPass>(value, RenderPassStr);
    }

    static const char* TextureBlendStr[] =
    {
        "NONE",
        "MIX",
        "MUL",
        "ADD",
        "SUB",
        "DIV",
        "DARK",
        "DIFF",
        "LIGHT",
        "SCREEN",
        "OVERLAY",
        "BLEND_HUE",
        "BLEND_SAT",
        "BLEND_VAL",
        "BLEND_COLOR"
    };

    const char* ToString(TextureBlend obj)
    {
        return ToString<TextureBlend>(obj, TextureBlendStr);
    }

    TextureBlend ToTextureBlend(const char* obj)
    {
        return ToType<TextureBlend>(obj, TextureBlendStr);
    }

    static const char* TextureTypeStr[] =
    {
        "UNKNOWN",
        "COL",
        "NORM",
        "COLSPEC",
        "COLMIR",
        "VARS",
        "REF",
        "SPEC",
        "EMIT",
        "ALPHA",
        "HAR",
        "RAYMIRR",
        "TRANSLU",
        "AMB",
        "DISPLACE",
        "WARP",
        "LAYER"
    };

    const char* ToString(TextureType obj)
    {
        return ToString<TextureType>(obj, TextureTypeStr);
    }

    TextureType ToTextureType(const char* obj)
    {
        return ToType<TextureType>(obj, TextureTypeStr);
    }

    static const char* CullFaceModeStr[] =
    {
        "BACK",
        "FRONT",
        "FRONT_AND_BACK", 
        "DISABLED"
    };

    const char* ToString(CullFaceMode obj)
    {
		return ToString<CullFaceMode>(obj, CullFaceModeStr);
    }

    CullFaceMode ToCullFaceMode(const char* value)
    {
        return ToType<CullFaceMode>(value, CullFaceModeStr);
    }

    static const char* BLEND_MODEStr[] =
    {
        "NONE",
        "ALPHA",
        "MULTIPLICATIVE",
        "ADDITIVE"
    };

	const char* ToString(BLEND_MODE obj)
	{
        return ToString<BLEND_MODE>(obj, BLEND_MODEStr);
	}

	BLEND_MODE ToBlendMode(const char* obj)
	{
        return ToType<BLEND_MODE>(obj, BLEND_MODEStr);
	}

	static const char* FillModeStr[] =
	{
		"SOLID",
		"WIREFRAME",
		"POINT"
	};

	const char* ToString(FillMode obj)
	{
		return ToString<FillMode>(obj, FillModeStr);
	}

	FillMode ToFillMode(const char* obj)
	{
		return ToType<FillMode>(obj, FillModeStr);
	}

	static const char* LightTypeStr[] =
	{
		"POINT", 
		"DIRECTIONAL", 
		"SPOT"
	};

	const char* ToString(LightType obj)
	{
		return ToString<LightType>(obj, LightTypeStr);
	}

	LightType ToLightType(const char* obj)
	{
		return ToType<LightType>(obj, LightTypeStr);
	}

	static const char* PhysicsShapeStr[] =
	{
		"SH_UNKNOWN",
		"SH_BOX",
		"SH_CONE_Z",
		"SH_CONE_Y",
		"SH_CONE_X",
		"SH_CYLINDER_Z",
		"SH_CYLINDER_Y",
		"SH_CYLINDER_X",
		"SH_CAPSULE_Z",
        "SH_CAPSULE_Y",
        "SH_CAPSULE_X",
		"SH_SPHERE",
		"SH_CONVEX_TRIMESH",
		"SH_TRIMESH"
	};

	const char* ToString(PhysicsShape obj)
	{
		return ToString<PhysicsShape>(obj, PhysicsShapeStr);
	}

	PhysicsShape ToPhysicsShape(const char* obj)
	{
		return ToType<PhysicsShape>(obj, PhysicsShapeStr);
	}

    static const char* TextureWrapModeStr[] =
    {
        "CLAMP_TO_EDGE",
        "MIRRORED_REPEAT",
        "REPEAT"
    };

    const char* ToString(TextureWrapMode obj)
    {
        return ToString<TextureWrapMode>(obj, TextureWrapModeStr);
    }

    TextureWrapMode ToTextureWrapMode(const char* obj)
    {
        return ToType<TextureWrapMode>(obj, TextureWrapModeStr);
    }

    static const char* TextureFilterModeStr[] =
    {
        "NEAREST",
        "BILINEAR",
        "REPEAT"
    };

    const char* ToString(TextureFilterMode obj)
    {
        return ToString<TextureFilterMode>(obj, TextureFilterModeStr);
    }

    TextureFilterMode ToTextureFilterMode(const char* obj)
    {
        return ToType<TextureFilterMode>(obj, TextureFilterModeStr);
    }

    std::string RemoveWhiteSpaces(const std::string& str)
    {
        std::string result(str);
        auto f = []( char ch ) { return std::isspace( ch ); };
        result.erase( std::remove_if( result.begin(), result.end(), f ), result.end() );
        return result;
    }

}