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

#include "Util.h"
#include "Check.h"
#include "Plane.h"
#include "Constants.h"
#include "Path.h"
#include "StringConverter.h"
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

    bool IsNaN(const Vector3& v)
    {
        return isnan(v.x) || isnan(v.y) || isnan(v.z);
    }

    Quaternion QuaternionFromLookRotation(const Vector3& direction, const Vector3& upDirection)
    {
        Vector3 forward = Normalize(direction);
        Vector3 v = Normalize(Cross(forward, upDirection));
        if (IsNaN(v))
        {
            auto up1 = upDirection;
            std::swap(up1.x, up1.y);
            v = Normalize(Cross(forward, up1));
            if (IsNaN(v))
            {
                up1 = upDirection;
                std::swap(up1.x, up1.z);
                v = Normalize(Cross(forward, up1));
                if (IsNaN(v))
                {
                    up1 = upDirection;
                    std::swap(up1.y, up1.z);
                    v = Normalize(Cross(forward, up1));
                    CHECK_ASSERT(!IsNaN(v));
                }
            }
        }
        Vector3 up = Cross(v, forward);
        Vector3 right = Cross(up, forward);

        Quaternion ret(Matrix3(right, up, forward));
        CHECK_ASSERT(!IsNaN(ret));
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

        Vertex3 scaling(Length(m[0]), Length(m[1]), Length(m[2]));
        Matrix3 tmp1(glm::scale(glm::mat4(1.0f), Vertex3(1) / scaling) * m);
        q = glm::quat_cast(tmp1);
        position = Vertex3(m[3]);
        Matrix3 tmp2(Inverse(tmp1) * Matrix3(m));
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
        Orthonormalize(matr);
        Quaternion rot(glm::quat_cast(m));
        if (Length2(rot) == 0.0)
            rot = QUATERNION_IDENTITY;
        q = rot;

        // extract scale
        Matrix3 m1 = Inverse(matr) * Matrix3(m);
        scale = Vertex3(m1[0][0], m1[1][1], m1[2][2]);

        #endif
    }

    Matrix4 ComposeMatrix(const Vertex3& position, const Quaternion& q, const Vertex3& scale)
    {
        return glm::translate(glm::mat4(), position) * glm::mat4_cast(q) * glm::scale(glm::mat4(1.0f), scale);
    }

    Matrix4 ComposeMatrix(const Vertex3& position, const Quaternion& q)
    {
        return glm::translate(glm::mat4(), position) * glm::mat4_cast(q);
    }

    Matrix4 ComposeMatrix(const Quaternion& q)
    {
        return glm::mat4_cast(q);
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

    unsigned NextPowerOfTwo(unsigned value)
    {
        unsigned ret = 1;
        while (ret < value && ret < 0x80000000)
            ret <<= 1;
        return ret;
    }

    bool IsZeroLength(const Vector3& obj)
    {
        auto length = Length(obj);
        return length <= EPSILON;
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
        CHECK_ASSERT(buffer.size() < std::numeric_limits<int>::max());
        compressBuffer.resize(LZ4_compressBound((int)buffer.size()));
        auto bufferSize = LZ4_compress(buffer.c_str(), &compressBuffer[0], (int)buffer.size());
        CHECK_ASSERT(bufferSize >= 0);
        compressBuffer.resize(bufferSize);
        return compressBuffer;
    }


    std::string DecompressBuffer(const std::string& buffer)
    {
        std::string smallBuffer;
        smallBuffer.resize(HeaderSize);
        CHECK_ASSERT(buffer.size() < std::numeric_limits<int>::max());
        CHECK_ASSERT(smallBuffer.size() < std::numeric_limits<int>::max());
        LZ4_decompress_safe_partial(&buffer[0], &smallBuffer[0], (int)buffer.size(), (int)smallBuffer.size(), (int)smallBuffer.size());
        std::string::size_type bytes = ToInt(smallBuffer);
        bytes += smallBuffer.size();
        std::string outputBuffer;
        outputBuffer.resize(bytes);
        CHECK_ASSERT(bytes < std::numeric_limits<int>::max());
        int totalBytes = LZ4_decompress_safe(&buffer[0], &outputBuffer[0], (int)buffer.size(), (int)bytes);
        CHECK_ASSERT(totalBytes == bytes);
        outputBuffer.erase(outputBuffer.begin(), outputBuffer.begin() + smallBuffer.size());
        return outputBuffer;
    }

    bool IsScaleUniform(const Vector3& scale)
    {
        return Abs(scale.x - scale.y) < PRECISION && Abs(scale.x - scale.z) < PRECISION;
    }

    Vector3 GetSlidingVector(const Vector3& dir2Target, const Vector3& hitNormal)
    {
        Vector3 reflection = glm::reflect(dir2Target, hitNormal);
        Vector3 parallelComponent = Dot(reflection, hitNormal) * hitNormal;
        Vector3 sliding = reflection - parallelComponent;
        return sliding;
    }

    float Dot(const Vector4& a, const Vector4& b)
    {
        return glm::dot(a, b);
    }

    float Dot(const Vector3& a, const Vector3& b)
    {
        return glm::dot(a, b);
    }

    float Dot(const Quaternion& a, const Quaternion& b)
    {
        return glm::dot(a, b);
    }

    Vector3 Cross(const Vector3& a, const Vector3& b)
    {
        return glm::cross(a, b);
    }

    float Min(float a, float b)
    {
        return std::min(a, b);
    }

    Matrix3 Transpose(const Matrix3& a)
    {
        return glm::transpose(a);
    }

    Matrix3 Inverse(const Matrix3& a)
    {
        return glm::inverse(a);
    }

    Matrix4 Inverse(const Matrix4& a)
    {
        return glm::inverse(a);
    }

    Matrix4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        return glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    Matrix4 Perspective(float fovyRadians, float aspectRatio, float zNear, float zFar)
    {
        return glm::perspective(fovyRadians, aspectRatio, zNear, zFar);
    }

    Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
    {
        return glm::lookAt(eye, center, up);
    }

    Quaternion Inverse(const Quaternion& a)
    {
        return glm::inverse(a);
    }

    Quaternion AngleAxis(float radians, const Vector3& axis)
    {
        return glm::angleAxis(radians, axis);
    }

    Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
    {
        return a * (1.0f - t) + b * t;
    }

    float Lerp(float a, float b, float t)
    {
        return a * (1.0f - t) + b * t;
    }

    Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t)
    {
        return glm::slerp(a, b, t);
    }

    float Clamp(float value, float minVal, float maxVal)
    {
        return glm::clamp(value, minVal, maxVal);
    }

    int Clamp(int value, int minVal, int maxVal)
    {
        return glm::clamp(value, minVal, maxVal);
    }

    float Length(const Vector3& value)
    {
        return glm::length(value);
    }

    float Length(const Vector4& value)
    {
        return glm::length(value);
    }

    float Length2(const Vector3& value)
    {
        return glm::length2(value);
    }

    float Radians(float degrees)
    {
        return glm::radians(degrees);
    }

    float Degrees(float radians)
    {
        return glm::degrees(radians);
    }

    Vector3 Radians(const Vector3& degrees)
    {
        return glm::radians(degrees);
    }

    Vector3 Degrees(const Vector3& radians)
    {
        return glm::degrees(radians);
    }


    float Angle(const Vector3& a, const Vector3& b)
    {
        return glm::angle(a, b);
    }

    Vector3 Normalize(const Vector3& value)
    {
        return glm::normalize(value);
    }

    Quaternion Normalize(const Quaternion& value)
    {
        return glm::normalize(value);
    }

    float Distance(float a, float b)
    {
        return glm::distance(a, b);
    }

    float Distance(const Vector4& a, const Vector4& b)
    {
        return glm::distance(a, b);
    }

    float Distance(const Vector3& a, const Vector3& b)
    {
        return glm::distance(a, b);
    }

    float Distance2(const Vector3& a, const Vector3& b)
    {
        return glm::distance2(a, b);
    }

    float Abs(float value)
    {
        return std::abs(value);
    }

    float Cos(float value)
    {
        return std::cos(value);
    }

    float Floor(float value)
    {
        return std::floor(value);
    }

    Vector3 Floor(Vector3 value)
    {
        return glm::floor(value);
    }

    Vector4 Row(const Matrix4& mat, int index)
    {
        return glm::row(mat, index);
    }

    Vector3 Row(const Matrix3 mat, int index)
    {
        return glm::row(mat, index);
    }

    Vector4 Column(const Matrix4& mat, int index)
    {
        return glm::column(mat, index);
    }

    Vector3 Column(const Matrix3& mat, int index)
    {
        return glm::column(mat, index);
    }

    Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float s)
    {
        return glm::catmullRom(v1, v2, v3, v4, s);
    }

    Vector4 Fract(const Vector4& value)
    {
        return glm::fract(value);
    }

    Vertex3 EulerAngles(const Quaternion& q)
    {
        return glm::eulerAngles(q);
    }

    Matrix4 GetSphericalBillboardMatrix(Matrix4 m)
    {
        m[0] = Vector4(1.0, 0.0, 0.0, m[0][3]);
        m[1] = Vector4(0.0, 1.0, 0.0, m[1][3]);
        m[2] = Vector4(0.0, 0.0, 1.0, m[2][3]);
        return m;
    }

    Matrix4 GetCylindricalBillboardMatrix(Matrix4 m)
    {
        m[0] = Vector4(1.0, 0.0, 0.0, m[0][3]);
        m[2] = Vector4(0.0, 0.0, 1.0, m[2][3]);
        return m;
    }

    #if 0
    void SleepMs(unsigned milliseconds)
    {
        #if EMSCRIPTEN
        emscripten_sleep(milliseconds);
        #else
        std::this_thread::sleep_for(Milliseconds(milliseconds));
        #endif
    }
    #endif

    Quaternion Rotation(const Vector3& a, const Vector3& b)
    {
        return glm::rotation(a, b);
    }
}