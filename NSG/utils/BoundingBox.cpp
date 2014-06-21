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
#include "BoundingBox.h"
#include "Node.h"
namespace NSG
{
	BoundingBox::BoundingBox(const Vertex3& min, const Vertex3& max)
	: min_(min),
	max_(max)
	{
	}


	BoundingBox::BoundingBox(const Node& node)
	{
		Vertex3 pos = node.GetGlobalPosition();
        Quaternion q = node.GetGlobalOrientation();
		Vertex3 scale = q * node.GetGlobalScale();
		min_ = pos - scale;
		max_ = pos + scale;
	}

	BoundingBox::~BoundingBox()
	{
	}

	Intersection BoundingBox::IsInside(const BoundingBox& box) const
	{
        if (box.max_.x < min_.x || box.min_.x > max_.x || 
        	box.max_.y < min_.y || box.min_.y > max_.y ||
            box.max_.z < min_.z || box.min_.z > max_.z)
        {
            return OUTSIDE;
        }
        else if (box.min_.x < min_.x || box.max_.x > max_.x || 
        	box.min_.y < min_.y || box.max_.y > max_.y ||
            box.min_.z < min_.z || box.max_.z > max_.z)
        {
            return INTERSECTS;
        }
        else
       	{
            return INSIDE;
        }
	}

	Intersection BoundingBox::IsInside(const Vertex3& point) const
	{
		if (point.x < min_.x || point.x > max_.x || 
			point.y < min_.y || point.y > max_.y ||
            point.z < min_.z || point.z > max_.z)
        {
            return OUTSIDE;
        }
        else
        {
            return INSIDE;
        }
	}

}