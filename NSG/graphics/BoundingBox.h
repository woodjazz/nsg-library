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
#pragma once
#include "Types.h"

namespace NSG
{
	struct BoundingBox
	{
		Vertex3 min_;
		Vertex3 max_;

	    BoundingBox();
		BoundingBox(const Vertex3& min, const Vertex3& max);
    	BoundingBox(const Vector3& point);
		BoundingBox(const Node& node);
		BoundingBox(const BoundingBox& obj);
		BoundingBox(float min, float max);
		~BoundingBox();
		void Define(const Vector3& point);
	    void Merge(const Vector3& point);
	    void Merge(const BoundingBox& box);
	    void Transform(const Node& node);
		Intersection IsInside(const BoundingBox& box) const;
		bool IsInside(const Vertex3& point) const;
	    Vector3 Center() const { return (max_ + min_) * 0.5f; }
	    Vector3 Size() const { return max_ - min_; }
	    void GetVertices(Vertex3 vertices[8]) const;
	};

	std::ostream& operator << (std::ostream& s , const BoundingBox& obj);

}