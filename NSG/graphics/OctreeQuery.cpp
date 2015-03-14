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
#include "OctreeQuery.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Frustum.h"

namespace NSG
{
    OctreeQuery::OctreeQuery(std::vector<SceneNode*>& result)
        : result_(result)
    {
    }

    OctreeQuery::~OctreeQuery()
    {
    }


    FrustumOctreeQuery::FrustumOctreeQuery(std::vector<SceneNode*>& result, const Camera* camera)
        : OctreeQuery(result),
          camera_(camera),
          frustum_(*camera->GetFrustumPointer())
    {
    }

    Intersection FrustumOctreeQuery::TestOctant(const BoundingBox& box, bool inside)
    {
        if (inside)
            return Intersection::INSIDE;
        else
            return frustum_.IsInside(box);
    }

    void FrustumOctreeQuery::Test(const std::vector<SceneNode*>& objs, bool inside)
    {
        for (auto& obj : objs)
        {
            if(camera_->GetLayer() != obj->GetLayer())
                continue;

            if (obj->GetMesh())
            {
                if (inside || frustum_.IsInside(obj->GetWorldBoundingBox()) != Intersection::OUTSIDE)
                    result_.push_back(obj);
            }
        }
    }

    RayOctreeQuery::RayOctreeQuery(std::vector<SceneNode*>& result, const Ray& ray)
        : OctreeQuery(result),
          ray_(ray)
    {
    }

    Intersection RayOctreeQuery::TestOctant(const BoundingBox& box, bool inside)
    {
        if (inside)
            return Intersection::INSIDE;
        else
            return ray_.IsInside(box);
    }

    void RayOctreeQuery::Test(const std::vector<SceneNode*>& objs, bool inside)
    {
        for (auto& obj : objs)
        {
            if(!obj->AllowRayQuery())
                continue;
            
            if (obj->GetMesh())
            {
                if (inside || ray_.IsInside(obj->GetWorldBoundingBox()) != Intersection::OUTSIDE)
                    result_.push_back(obj);
            }
        }
    }
}
