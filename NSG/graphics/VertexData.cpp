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
#include "VertexData.h"

namespace NSG
{
    VertexData::VertexData()
        : normal_(0, 0, 1),  // always facing forward,
          color_(1)

    {
    }

    bool VertexData::operator == (const VertexData& obj) const
    {
        return position_ == obj.position_ &&
               normal_ == obj.normal_ &&
               uv0_ == obj.uv0_ &&
               uv1_ == obj.uv1_ &&
               color_ == obj.color_ &&
               tangent_ == obj.tangent_ &&
               bonesID_ == obj.bonesID_ &&
               bonesWeight_ == obj.bonesWeight_;
    }

    std::ostream& operator << (std::ostream& s , const VertexData& obj)
    {
        s << obj.position_ << obj.normal_ << obj.uv0_ << obj.uv1_ << obj.color_ << obj.tangent_ << obj.bonesID_ << obj.bonesWeight_;
        return s;
    }
}