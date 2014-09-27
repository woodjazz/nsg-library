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

#include "Util.h"
#include "Constants.h"
#include <string>
#include <algorithm>
#include <cctype>

namespace NSG
{
    void DecomposeMatrix(const Matrix4& m, Vertex3& position, Quaternion& q, Vertex3& scale)
    {
        Vertex3 scaling(glm::length(m[0]), glm::length(m[1]), glm::length(m[2]));

        Matrix3 tmp1(glm::scale(glm::mat4(1.0f), Vertex3(1) / scaling) * m);

        q = glm::quat_cast(tmp1);

        position = Vertex3(m[3]);

        Matrix3 tmp2(glm::inverse(tmp1) * Matrix3(m));

        scale = Vertex3(tmp2[0].x, tmp2[1].y, tmp2[2].z);
    }

    void ReplaceChar(std::string& source, char from, char to)
    {
        for (;;)
        {
            const size_t last_slash_idx = source.find_last_of(from);
            if (std::string::npos == last_slash_idx) break;
            source.replace(last_slash_idx, 1, 1, to);
        }
    }

    std::string GetLowercaseFileExtension(const std::string& filename)
    {
        std::string extension;
        std::string::size_type pos = filename.find_last_of(".");
        if (pos != std::string::npos)
        {
            std::copy(filename.begin() + pos, filename.end(), std::back_inserter(extension));
            for (auto& ch : extension)
                ch = std::tolower(ch);
        }

        return extension;
    }

	std::istream& operator >> (std::istream& s, Vertex2& obj)
	{
		char ch;
		s >> ch;
		CHECK_ASSERT(ch == '[', __FILE__, __LINE__);
		s >> obj.x;
		s >> ch;
		CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
		s >> obj.y;
		s >> ch;
		CHECK_ASSERT(ch == ']', __FILE__, __LINE__);

		return s;
	}

    std::istream& operator >> (std::istream& s , Vertex3& obj)
    {
    	char ch;
        s >> ch;
        CHECK_ASSERT(ch == '[', __FILE__, __LINE__);
        s >> obj.x;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.y;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.z;
        s >> ch;
        CHECK_ASSERT(ch == ']', __FILE__, __LINE__);

		return s;
    }

    std::istream& operator >> (std::istream& s , Vertex4& obj)
    {
        char ch;
        s >> ch;
        CHECK_ASSERT(ch == '[', __FILE__, __LINE__);
        s >> obj.x;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.y;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.z;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.w;
        s >> ch;
        CHECK_ASSERT(ch == ']', __FILE__, __LINE__);

		return s;
    }

	Vertex2 GetVertex2(const std::string& buffer)
	{
		std::stringstream ss;
		ss << buffer;
		Vertex2 obj;
		ss >> obj;
		return obj;
	}


    Vertex3 GetVertex3(const std::string& buffer)
    {
    	std::stringstream ss;
    	ss << buffer;
    	Vertex3 obj;
    	ss >> obj;
    	return obj;
    }

    Vertex4 GetVertex4(const std::string& buffer)
    {
        std::stringstream ss;
        ss << buffer;
        Vertex4 obj;
        ss >> obj;
        return obj;
    }

    std::istream& operator >> (std::istream& s , Quaternion& obj)
    {
    	char ch;
        s >> ch;
        CHECK_ASSERT(ch == '[', __FILE__, __LINE__);
        s >> obj.w;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.x;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.y;
        s >> ch;
        CHECK_ASSERT(ch == ',', __FILE__, __LINE__);
        s >> obj.z;
        s >> ch;
        CHECK_ASSERT(ch == ']', __FILE__, __LINE__);

		return s;
    }

    Quaternion GetQuaternion(const std::string& buffer)
    {
    	std::stringstream ss;
    	ss << buffer;
    	Quaternion obj;
    	ss >> obj;
    	return obj;
    }
#if 0
	float CalculateArcBallAnglesForPoint(const Vertex3& center, const Vertex3& currentPoint, float& theta, float& phi)
	{
		float radius = glm::distance(currentPoint, center);

		CHECK_ASSERT(radius > 0.05f, __FILE__, __LINE__);

		Vector3 rn = glm::normalize(currentPoint - center);

		//calculate phi in order to be in the current position
		float dy = rn.y;
		phi = acos(dy); 

		//calculate theta in order to be in the current position
		float dx = rn.x;
		float dz = rn.z;

		theta = atan(dz / dx);

		if (dx < 0)
			theta += PI;

		//TRACE_LOG("theta=" << glm::degrees(theta) << " phi=" << glm::degrees(phi));

		return radius;
	}

    Vertex3 CalculateArcBallPoint(const Vertex3& center, float radius, Vector3& up, float theta, float phi)
    {
        // Apply spherical coordinates
		Vertex3 newPoint(cos(theta) * sin(phi), cos(phi), sin(theta) * sin(phi));

        // Reduce theta slightly to obtain another point on the same longitude line on the sphere.
		const float dt = 1;
		Vertex3 newUpPoint(cos(theta) * sin(phi - dt), cos(phi - dt), sin(theta) * sin(phi - dt));
        up = glm::normalize(newUpPoint - newPoint);

		return center + radius * newPoint;
    }
#endif

}
