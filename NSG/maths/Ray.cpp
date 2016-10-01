//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Updated by Néstor Silveira Gorski for nsg-library

#include "Ray.h"
#include "BoundingBox.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "Check.h"
#include "Maths.h"

namespace NSG
{
    Ray::Ray(const Vertex3& origin, const Vector3& direction, float maxDistance)
        : origin_(origin),
          direction_(direction.Normalize()),
          maxDistance_(maxDistance)
    {

    }

    Ray::Ray(const Ray& ray) :
        origin_(ray.origin_),
        direction_(ray.direction_),
        maxDistance_(ray.maxDistance_)
    {
    }

    Ray::~Ray()
    {

    }

    Ray& Ray::operator = (const Ray& rhs)
    {
        origin_ = rhs.origin_;
        direction_ = rhs.direction_;
        maxDistance_ = rhs.maxDistance_;
		maxDistance_ = rhs.maxDistance_;
        return *this;
    }

    bool Ray::operator == (const Ray& rhs) const
    {
        return origin_ == rhs.origin_ && direction_ == rhs.direction_ && maxDistance_ == rhs.maxDistance_;
    }

    bool Ray::operator != (const Ray& rhs) const
    {
        return origin_ != rhs.origin_ || direction_ != rhs.direction_ || maxDistance_ != rhs.maxDistance_;
    }

    Intersection Ray::IsInside(const BoundingBox& box) const
    {
        if (HitDistance(box) < maxDistance_)
            return Intersection::INTERSECTS;
        return Intersection::OUTSIDE;
    }

    float Ray::HitDistance(const Vector3& v0, const Vector3& v1, const Vector3& v2, Vector3* outNormal) const
    {
        // Based on Fast, Minimum Storage Ray/Triangle Intersection by Möller & Trumbore
        // http://www.graphics.cornell.edu/pubs/1997/MT97.pdf
        // Calculate edge vectors
        Vector3 edge1(v1 - v0);
        Vector3 edge2(v2 - v0);

        // Calculate determinant & check backfacing
        Vector3 p(direction_.Cross(edge2));
        float det = edge1.Dot(p);
        if (det >= std::numeric_limits<float>::epsilon())
        {
            // Calculate u & v parameters and test
            Vector3 t(origin_ - v0);
            float u = t.Dot(p);
            if (u >= 0.0f && u <= det)
            {
                Vector3 q(t.Cross(edge1));
                float v = direction_.Dot(q);
                if (v >= 0.0f && u + v <= det)
                {
                    // There is an intersection, so calculate distance & optional normal
                    if (outNormal)
                        *outNormal = edge1.Cross(edge2);

                    return edge2.Dot(q) / det;
                }
            }
        }

        return std::numeric_limits<float>::max();
    }


    float Ray::HitDistance(const BoundingBox& box) const
    {
        if (box.IsInside(origin_))
            return 0;

        float distance = maxDistance_;

        if (origin_.x < box.min_.x && direction_.x > 0.0f)
        {
            float x = (box.min_.x - origin_.x) / direction_.x;
            if (x < distance)
            {
                Vector3 point = origin_ + x * direction_;
                point = (point * 10000.0f + 0.5f).Floor() / 10000.0f; //round value to 4 decimal points
                if (point.y >= box.min_.y && point.y <= box.max_.y && point.z >= box.min_.z && point.z <= box.max_.z)
                    distance = x;
            }
        }
        if (origin_.x > box.max_.x && direction_.x < 0.0f)
        {
            float x = (box.max_.x - origin_.x) / direction_.x;
            if (x < distance)
            {
                Vector3 point = origin_ + x * direction_;
                point = (point * 10000.0f + 0.5f).Floor() / 10000.0f; //round value to 4 decimal points
                if (point.y >= box.min_.y && point.y <= box.max_.y && point.z >= box.min_.z && point.z <= box.max_.z)
                    distance = x;
            }
        }
        // Check for intersecting in the Y-direction
        if (origin_.y < box.min_.y && direction_.y > 0.0f)
        {
            float x = (box.min_.y - origin_.y) / direction_.y;
            if (x < distance)
            {
                Vector3 point = origin_ + x * direction_;
                point = (point * 10000.0f + 0.5f).Floor() / 10000.0f; //round value to 4 decimal points
                if (point.x >= box.min_.x && point.x <= box.max_.x && point.z >= box.min_.z && point.z <= box.max_.z)
                    distance = x;
            }
        }
        if (origin_.y > box.max_.y && direction_.y < 0.0f)
        {
            float x = (box.max_.y - origin_.y) / direction_.y;
            if (x < distance)
            {
                Vector3 point = origin_ + x * direction_;
                point = (point * 10000.0f + 0.5f).Floor() / 10000.0f; //round value to 4 decimal points
                if (point.x >= box.min_.x && point.x <= box.max_.x && point.z >= box.min_.z && point.z <= box.max_.z)
                    distance = x;
            }
        }
        // Check for intersecting in the Z-direction
        if (origin_.z < box.min_.z && direction_.z > 0.0f)
        {
            float x = (box.min_.z - origin_.z) / direction_.z;
            if (x < distance)
            {
                Vector3 point = origin_ + x * direction_;
                point = (point * 10000.0f + 0.5f).Floor() / 10000.0f; //round value to 4 decimal points
                if (point.x >= box.min_.x && point.x <= box.max_.x && point.y >= box.min_.y && point.y <= box.max_.y)
                    distance = x;
            }
        }
        if (origin_.z > box.max_.z && direction_.z < 0.0f)
        {
            float x = (box.max_.z - origin_.z) / direction_.z;
            if (x < distance)
            {
                Vector3 point = origin_ + x * direction_;
                point = (point * 10000.0f + 0.5f).Floor() / 10000.0f; //round value to 4 decimal points
                if (point.x >= box.min_.x && point.x <= box.max_.x && point.y >= box.min_.y && point.y <= box.max_.y)
                    distance = x;
            }
        }

        return distance;
    }

    float Ray::HitDistance(const SceneNode* node) const
    {
		const float MAX_DISTANCE = std::numeric_limits<float>::max();
		float nearest = MAX_DISTANCE;
        PMesh mesh = node->GetMesh();
        if (mesh)
        {
			auto& m = node->GetGlobalModelInvMatrix();
            Ray localRay = Transformed(m);
			size_t n = mesh->GetNumberOfTriangles();
            for(size_t i = 0; i<n; i++)
            {
				const Vector3& v0 = mesh->GetTriangleVertex(i, 0).position_;
				const Vector3& v1 = mesh->GetTriangleVertex(i, 1).position_;
				const Vector3& v2 = mesh->GetTriangleVertex(i, 2).position_;
				nearest = Min(nearest, localRay.HitDistance(v0, v1, v2));
            }

			if (nearest < MAX_DISTANCE)
			{
				// multiply per scale to have correct distance
				Vector3 scale = node->GetGlobalScale();
				CHECK_ASSERT(Abs(scale.x) > EPSILON);
				auto maxScale = std::max(std::max(scale.x, scale.y), scale.z);
				nearest *= maxScale;
			}
        }
        return nearest;
    }

    Vertex3 Ray::GetPoint(float distance) const
    {
        return origin_ + direction_ * distance;
    }

    Ray Ray::Transformed(const Matrix4& transform) const
    {
		Ray ret(Vertex3(transform * Vector4(origin_, 1)), Vertex3(transform * Vector4(direction_, 0.0f)));
        return ret;
    }

}
