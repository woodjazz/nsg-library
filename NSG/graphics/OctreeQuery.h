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

#pragma once
#include "Types.h"
#include "Frustum.h"
#include "Ray.h"
#include <vector>

namespace NSG
{
    class OctreeQuery
    {
    public:
        OctreeQuery(std::vector<SceneNode*>& result);
        virtual ~OctreeQuery();
        virtual Intersection TestOctant(const BoundingBox& box, bool inside) = 0;
        virtual void Test(const std::vector<SceneNode*>& objs, bool inside) = 0;
        std::vector<SceneNode*>& result_;
    };

    class FrustumOctreeQuery : public OctreeQuery
    {
    public:
        FrustumOctreeQuery(std::vector<SceneNode*>& result, const Frustum* frustum);
        virtual Intersection TestOctant(const BoundingBox& box, bool inside) override;
        virtual void Test(const std::vector<SceneNode*>& objs, bool inside) override;
    private:
        const Frustum* frustum_;
    };

    class  RayOctreeQuery  : public OctreeQuery
    {
    public:
        RayOctreeQuery(std::vector<SceneNode*>& result, const Ray& ray);
        virtual Intersection TestOctant(const BoundingBox& box, bool inside) override;
        virtual void Test(const std::vector<SceneNode*>& objs, bool inside) override;
    private:
        Ray ray_;
    };

}