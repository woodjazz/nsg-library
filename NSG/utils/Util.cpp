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

#include "Util.h"
#include "Plane.h"
#include "Constants.h"
#include "Path.h"
#include "pugixml.hpp"
#include "lz4.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <thread>
#ifndef WIN32
#include <unistd.h>
#include <cerrno>
#else
#include <windows.h>
#define snprintf _snprintf
#endif
#if EMSCRIPTEN
#include "emscripten.h"
#endif

namespace NSG
{
    btTransform ToTransform(const Vector3& pos, const Quaternion& rot)
    {
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
        trans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
        return trans;
    }

    btVector3 ToBtVector3(const Vector3& obj)
    {
        return btVector3(obj.x, obj.y, obj.z);
    }

    Vector3 ToVector3(const btVector3& obj)
    {
        return Vector3(obj.x(), obj.y(), obj.z());
    }

    btQuaternion ToBtQuaternion(const Quaternion& q)
    {
        return btQuaternion(q.x, q.y, q.z, q.w);
    }

    bool IsNaN(const Quaternion& q)
    {
        return isnan(q.w) || isnan(q.x) || isnan(q.y) || isnan(q.z);
    }

    Quaternion QuaternionFromLookRotation(const Vector3& direction, const Vector3& upDirection)
    {
        Vector3 forward = glm::normalize(direction);
        Vector3 v = glm::normalize(glm::cross(forward, upDirection));
        Vector3 up = glm::cross(v, forward);
        Vector3 right = glm::cross(up, forward);

        Quaternion ret(Matrix3(right, up, forward));
        CHECK_ASSERT(!IsNaN(ret), __FILE__, __LINE__);
        return ret;
    }

    Quaternion ToQuaternion(const btQuaternion& q)
    {
        return Quaternion(q.w(), q.x(), q.y(), q.z());
    }

    /// Return the translation part.
    Vector3 Translation(const Matrix4& m)
    {
        return Vector3(m[3]);
    }

    /// Return the scaling part.
    Vector3 Scale(const Matrix4& m)
    {
        return Vector3(
                   sqrtf(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2]),
                   sqrtf(m[1][0] * m[1][0] + m[1][1] * m[1][1] + m[1][2] * m[1][2]),
                   sqrtf(m[2][0] * m[2][0] + m[2][1] * m[2][1] + m[2][2] * m[2][2])
               );
    }

    void DecomposeMatrix(const Matrix4& m, Vertex3& position, Quaternion& q, Vertex3& scale)
    {
        #if 1
        
		Vertex3 scaling(glm::length(m[0]), glm::length(m[1]), glm::length(m[2]));
        Matrix3 tmp1(glm::scale(glm::mat4(1.0f), Vertex3(1) / scaling) * m);
        q = glm::quat_cast(tmp1);
        position = Vertex3(m[3]);
        Matrix3 tmp2(glm::inverse(tmp1) * Matrix3(m));
        scale = Vertex3(tmp2[0].x, tmp2[1].y, tmp2[2].z);
        if (IsZeroLength(scale))
            scale = Vector3(1);// prevent zero scale

        #elif DECOM0
        Vector3 skew;
        Vector4 perspective;
        glm::decompose(m, scale, q, position, skew, perspective);

        #else
        // extract translation
        position = Vertex3(m[3]);

        // extract rotation
        Matrix3 matr(m);
        glm::orthonormalize(matr);
        Quaternion rot(glm::quat_cast(m));
        if (glm::length2(rot) == 0.0)
            rot = QUATERNION_IDENTITY;
        q = rot;

        // extract scale
        Matrix3 m1 = glm::inverse(matr) * Matrix3(m);
        scale = Vertex3(m1[0][0], m1[1][1], m1[2][2]);

        #endif
    }


    std::string GetUniqueName(const std::string& name)
    {
        static int counter = 0;
        const int MaxBuffer = 100;
        char buffer[MaxBuffer];
        if (name == "")
            snprintf(buffer, MaxBuffer, "Gen%d", counter++);
        else
            snprintf(buffer, MaxBuffer, "%s%d", name.c_str(), counter++);
        return buffer;
    }

    void GetPowerOfTwoValues(int& width, int& height)
    {
        int new_width = 1;
        int new_height = 1;

        while (new_width < width)
            new_width *= 2;

        while (new_height < height)
            new_height *= 2;

        width = new_width;
        height = new_height;
    }

    bool IsPowerOfTwo(int value)
    {
        return (!(value & (value - 1)) && value);
    }

    bool IsZeroLength(const Vector3& obj)
    {
        auto length = glm::length(obj);
        return length <= glm::epsilon<float>();
    }

    GLushort Transform(GLubyte selected[4])
    {
        GLushort b3 = (GLushort)selected[3] / 0x10;
        GLushort b2 = (GLushort)selected[2] / 0x10;
        GLushort b1 = (GLushort)selected[1] / 0x10;
        GLushort b0 = (GLushort)selected[0] / 0x10;
        GLushort index = b3 << 12 | b2 << 8 | b1 << 4 | b0;
        return index;
    }

    Color Transform(GLushort id)
    {
        Color color;
        color[0] = (id & 0x000F) / 15.0f;
        color[1] = ((id & 0x00F0) >> 4) / 15.0f;
        color[2] = ((id & 0x0F00) >> 8) / 15.0f;
        color[3] = ((id & 0xF000) >> 12) / 15.0f;
        return color;
    }

    static size_t HeaderSize = 128;
    std::string CompressBuffer(const std::string& buf)
    {
        std::string buffer = ToString(buf.size()) + " ";
        buffer.resize(HeaderSize);
        buffer += buf;
        std::string compressBuffer;
        CHECK_ASSERT(buffer.size() < std::numeric_limits<int>::max(), __FILE__, __LINE__);
        compressBuffer.resize(LZ4_compressBound((int)buffer.size()));
        auto bufferSize = LZ4_compress(buffer.c_str(), &compressBuffer[0], (int)buffer.size());
        CHECK_ASSERT(bufferSize >= 0, __FILE__, __LINE__);
        compressBuffer.resize(bufferSize);
        return compressBuffer;
    }


    std::string DecompressBuffer(const std::string& buffer)
    {
        std::string smallBuffer;
        smallBuffer.resize(HeaderSize);
        CHECK_ASSERT(buffer.size() < std::numeric_limits<int>::max(), __FILE__, __LINE__);
        CHECK_ASSERT(smallBuffer.size() < std::numeric_limits<int>::max(), __FILE__, __LINE__);
        LZ4_decompress_safe_partial(&buffer[0], &smallBuffer[0], (int)buffer.size(), (int)smallBuffer.size(), (int)smallBuffer.size());
        std::string::size_type bytes = ToInt(smallBuffer);
        bytes += smallBuffer.size();
        std::string outputBuffer;
        outputBuffer.resize(bytes);
        CHECK_ASSERT(bytes < std::numeric_limits<int>::max(), __FILE__, __LINE__);
        int totalBytes = LZ4_decompress_safe(&buffer[0], &outputBuffer[0], (int)buffer.size(), (int)bytes);
        CHECK_ASSERT(totalBytes == bytes, __FILE__, __LINE__);
        outputBuffer.erase(outputBuffer.begin(), outputBuffer.begin() + smallBuffer.size());
        return outputBuffer;
    }

    bool IsScaleUniform(const Vector3& scale)
    {
        return glm::abs(scale.x - scale.y) < PRECISION && glm::abs(scale.x - scale.z) < PRECISION;
    }


    #if 0
    void Sleep(unsigned milliseconds)
    {
        #if EMSCRIPTEN
        emscripten_sleep(milliseconds);
        #else
        std::this_thread::sleep_for(Milliseconds(milliseconds));
        #endif
    }
    #endif


}