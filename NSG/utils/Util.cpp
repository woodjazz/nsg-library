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
#include "Path.h"
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#ifndef WIN32
#include <unistd.h>
#include <cerrno>
#else
#include "windows.h"
#endif

namespace NSG
{
    Vector3 Lerp(const Vector3& lhs, const Vector3& rhs, float t)
    {
        return lhs * (1.0f - t) + rhs * t;
    }

    void DecomposeMatrix(const Matrix4& m, Vertex3& position, Quaternion& q, Vertex3& scale)
    {
        Vertex3 scaling(glm::length(m[0]), glm::length(m[1]), glm::length(m[2]));

        Matrix3 tmp1(glm::scale(glm::mat4(1.0f), Vertex3(1) / scaling) * m);

        q = glm::quat_cast(tmp1);

        position = Vertex3(m[3]);

        Matrix3 tmp2(glm::inverse(tmp1) * Matrix3(m));

        scale = Vertex3(tmp2[0].x, tmp2[1].y, tmp2[2].z);

        // prevent zero scale
        if(IsZeroLength(scale))
            scale = Vector3(1);
    }

    bool NSGCopyFile(const Path& source, const Path& target)
    {
        std::ifstream is(source.GetFullAbsoluteFilePath(), std::ios::binary);
        if (is.is_open())
        {
            std::ofstream os(target.GetFullAbsoluteFilePath(), std::ios::binary);
            if (os.is_open())
            {
                os << is.rdbuf();
                return true;
            }
        }
        return false;
    }

    std::istream& operator >> (std::istream& s, Vertex2& obj)
    {
        char ch(0);
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

    std::istream& operator >> (std::istream& s, Vertex3& obj)
    {
        char ch(0);
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

    std::istream& operator >> (std::istream& s, Vertex4& obj)
    {
        char ch(0);
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

    Matrix4 GetMatrix4(const std::string& buffer)
    {
        std::stringstream ss;
        ss << buffer;
        Matrix4 obj;
        ss >> obj;
        return obj;
    }

    std::string ToString(const Matrix4& m)
    {
        std::stringstream ss;
        ss << '[';
        ss << glm::column(m, 0);
        ss << glm::column(m, 1);
        ss << glm::column(m, 2);
        ss << glm::column(m, 3);
        ss << ']';

        return ss.str();
    }

    std::istream& operator >> (std::istream& s, Matrix4& obj)
    {
        char ch(0);
        s >> ch;
        CHECK_ASSERT(ch == '[', __FILE__, __LINE__);
        Vector4 col0;
        s >> col0;
        Vector4 col1;
        s >> col1;
        Vector4 col2;
        s >> col2;
        Vector4 col3;
        s >> col3;
        s >> ch;
        CHECK_ASSERT(ch == ']', __FILE__, __LINE__);

        obj = Matrix4(col0, col1, col2, col3);

        return s;
    }


    std::istream& operator >> (std::istream& s, Quaternion& obj)
    {
        char ch(0);
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

    bool SetCurrentDir(const std::string& path)
    {
        if (!path.empty())
        {
            #ifdef WIN32
            {
                if (SetCurrentDirectory(path.c_str()) == FALSE)
                {
                    TRACE_LOG("Failed to change directory to " << path << " with error = " << GetLastError());
                    return false;
                }
            }
            #else
            {
                if (chdir(path.c_str()) != 0)
                {
                    TRACE_LOG("Failed to change directory to " << path << " with error = " << errno);
                    return false;
                }
            }
            #endif
        }
        return true;
    }

    std::string GetUniqueName(const std::string& name)
    {
        static int counter = 0;
        std::stringstream ss;
        if (name.empty())
            ss << "GeneratedName";
        ss << counter++;
        return name + ss.str();
    }

    void GetPowerOfTwoValues(unsigned& width, unsigned& height)
    {
        unsigned new_width = 1;
        unsigned new_height = 1;

        while (new_width < width)
            new_width *= 2;

        while (new_height < height)
            new_height *= 2;

        width = new_width;
        height = new_height;
    }

    bool IsPowerOfTwo(unsigned value)
    {
        return (!(value & (value - 1)) && value);
    }

    bool IsZeroLength(const Vector3& obj)
    {
        auto length = glm::length(obj);
        return length <= glm::epsilon<float>();
    }
}